﻿#define _CRT_SECURE_NO_WARNINGS


#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

#include "./DataStructureModule.h"
using namespace std;

//TODO delete 中不允许完整属性名


// https://stackoverflow.com/questions/4266914/how-does-a-const-struct-differ-from-a-struct
// 根据这个链接，const 可以删
struct Datatype
{
	uint8_t id;
	uint32_t size;
};
const Datatype FieldType_INT = { 1, 4 };
const Datatype FieldType_FLOAT = { 3, 8 };
const Datatype FieldType_CHAR = { 10, 1 };
const Datatype FieldType_BYTE = { 11, 1 };

Field* newField[100];

string strip(const string& str) {
	if (str == "") {
		return "";
	}
	auto begin = str.find_first_not_of(' ');
	auto end = str.find_last_not_of(' ');
	return str.substr(begin, end - begin + 1);
}

vector<string> split(const string& str, const string& delim) {
	vector<string> res;
	if ("" == str) return res;
	//先将要切割的字符串从string类型转换为char*类型  
	char* strs = new char[str.length() + 1];
	strcpy(strs, str.c_str());

	char* d = new char[delim.length() + 1];
	strcpy(d, delim.c_str());

	char* p = strtok(strs, d);
	while (p) {
		string s = p; //分割得到的字符串转换为string类型
		s = strip(s);
		res.push_back(s); //存入结果数组  
		p = strtok(NULL, d);
	}
	delete[] strs;
	delete[] d;
	return res;
}

void LogInfo(string msg, uint8_t level) {
	if (level < 5) {
		printf("Info( level=%d ) : %s\n", level, msg.c_str());
	}
	if (level < 10) {
		printf("Warning( level=%d ) : %s\n", level, msg.c_str());
	}
	else {
		printf("Error( level=%d ) : %s\n", level, msg.c_str());
	}
}



//属性名+类型号+长度+标签
#define FIELD_SIZE (MAX_FIELD_NAME_LEN+sizeof(uint8_t)+sizeof(uint32_t)+sizeof(uint32_t))
class Field
{
public:
	Field(string fieldName, Datatype fieldType, uint32_t length, uint32_t FieldProperty = 0) {
		this->FieldName = fieldName;
		this->FieldType = fieldType;
		if (fieldType.id == FieldType_CHAR.id || fieldType.id == FieldType_BYTE.id) {
			this->FieldSize = length;
		}
		else {
			this->FieldSize = fieldType.size;
		}
		if ((FieldProperty & FIELD_PROPERTY_PK) != 0) {
			FieldProperty = FieldProperty | FIELD_PROPERTY_NOT_NULL;
		}
		this->FieldProperty = FieldProperty;
	}
	Field(Field* f) {
		this->FieldName = f->FieldName;
		this->FieldProperty = f->FieldProperty;
		this->FieldSize = f->FieldSize;
		this->FieldType = f->FieldType;
	}
	string FieldName;
	Datatype FieldType = FieldType_INT;
	uint32_t FieldProperty = 0;
	uint32_t FieldSize = 1;
	uint32_t Offset = 0;
};

//表名+属性数量+数据数量+数据长度
#define TABLE_HEAD_SIZE (MAX_TABLE_NAME_LEN+sizeof(uint32_t)+sizeof(uint64_t)+sizeof(uint32_t))

class Table
{

public:

	string TableName;
	Field* TableField[MAX_FIELD_NUMBER];
	map<string, Field*> FieldMap = map<string, Field*>();
	uint32_t TableFieldNum = 0;
	uint32_t TableRowSize = 0;
	vector<uint8_t*> Data = vector<uint8_t*>();

	Table() {
		memset(this->TableField, 0, MAX_FIELD_NUMBER);
		return;
	}
	Table(string tableName) {
		this->TableName = tableName;
		memset(this->TableField, 0, MAX_FIELD_NUMBER);
	}
	~Table() {
		for (uint32_t i = 0; i < this->TableFieldNum; ++i) {
			delete this->TableField[i];
		}
		for (auto iter1 = this->Data.begin(); iter1 != this->Data.end(); iter1++) {
			free(*iter1);
		}
		this->Data.clear();
	}

	void AddField(Field* f) {
		if (TableFieldNum == MAX_FIELD_NUMBER) {
			LogInfo("Field number exceed", 12);
			return;
		}
		else {
			this->TableField[TableFieldNum] = f;
			if (TableFieldNum != 0) {
				f->Offset = this->TableField[TableFieldNum - 1]->Offset + TableField[TableFieldNum - 1]->FieldSize;
			}
			else {
				f->Offset = 0;
			}
			this->TableRowSize += f->FieldSize;
			TableFieldNum++;
			FieldMap[f->FieldName] = f;
			return;
		}
	}

	/// <summary>
	/// 将数据插入表中（深拷贝）
	/// </summary>
	/// <param name="DataTuple">指向数据头的指针</param>
	/// <returns>返回该表</returns>
	Table* InsertData(uint8_t* data) {
		uint8_t* pDataRow = (uint8_t*)calloc(this->TableRowSize, sizeof(uint8_t));
		if (pDataRow == NULL) {
			LogInfo("Unable to allocate memory", 12);
			return nullptr;
		}
		memcpy(pDataRow, data, this->TableRowSize);
		this->Data.push_back(pDataRow);
		return this;
	}
	/// <summary>
	/// 删除表中的数据
	/// </summary>
	/// <param name="rowIndex">待删除的行号</param>
	/// <returns>返回该表</returns>
	Table* DeleteData(uint64_t rowIndex) {
		auto p = begin(this->Data) + rowIndex;
		free(*p);
		this->Data.erase(p);
		return this;
	}

	Table* UpdateData(uint8_t* pData, uint32_t offset, uint32_t length, uint64_t rowIndex) {
		memcpy(this->Data[rowIndex] + offset, pData, length);
		return this;
	}

	vector<uint64_t>* WhereFilter(const string& whereStr) {

		//Score.Mark >= 60 and Score.Mark < 90
		if (whereStr == "") {
			auto pRet = new vector<uint64_t>();
			for (uint64_t i = 0; i < this->Data.size(); ++i) {
				pRet->push_back(i);
			}
			return pRet;
		}

		//TODO 默认在关键字之间会添加空格，计划自动添加空格
		string UpperWhere = whereStr;
		transform(UpperWhere.begin(), UpperWhere.end(), UpperWhere.begin(), ::toupper);

		auto token = split(whereStr, " ");
		auto UpperToken = split(UpperWhere, " ");
		vector<uint8_t> tokenType = vector<uint8_t>(token.size());

		vector<string> postFix;
		stack<string>buff;
		//make postfix
		for (uint32_t i = 0; i < token.size(); ++i) {
			if (UpperToken[i] == "<" || UpperToken[i] == "<=" ||
				UpperToken[i] == "=" || UpperToken[i] == ">=" ||
				UpperToken[i] == ">" || UpperToken[i] == "!=" ||
				UpperToken[i] == "AND" || UpperToken[i] == "OR") {
				if (buff.size() == 0) {
					buff.push(token[i]);
				}
				else
				{
					while ((
						(UpperToken[i] == "AND" || UpperToken[i] == "OR")
						&& buff.size() != 0
						&& (buff.top() != "AND" && buff.top() != "OR"))
						)
					{
						postFix.push_back(buff.top());
						buff.pop();
					}
					buff.push(UpperToken[i]);
				}
			}
			else {
				postFix.push_back(token[i]);
			}
		}
		while (buff.size() != 0)
		{
			postFix.push_back(buff.top());
			buff.pop();
		}
		//cal

		vector<uint64_t>* result = new vector<uint64_t>;
		if (result == NULL) {
			LogInfo("Unable to allocate memory", 12);
			return nullptr;
		}
		stack<string> res;
		for (uint64_t index = 0; index < this->Data.size(); index++)
		{
			uint8_t* data = this->Data[index];
			for (uint32_t i = 0; i < postFix.size(); i++)
			{
				if (postFix[i] == "<" || postFix[i] == "<=" ||
					postFix[i] == "=" || postFix[i] == ">=" ||
					postFix[i] == ">" || postFix[i] == "!=" ||
					postFix[i] == "AND" || postFix[i] == "OR")
				{
					if (res.size() < 2) {
						LogInfo("Syntax error while analyzing SQL : " + whereStr + ".", 16);
						return nullptr;
					}
					string a = res.top(); res.pop();
					string b = res.top(); res.pop();
					string op = postFix[i];
					string ans;

					uint8_t typeIdA = 0;
					uint8_t typeIdB = 0;
					uint32_t beginA, lengthA, beginB, lengthB;
					double diff;
					if (op == "OR") {
						if (a == "TRUE" || b == "TRUE") {
							ans = "TRUE";
						}
						else if (a == "FALSE") {
							ans = b;
						}
						else if (b == "FALSE") {
							ans = a;
						}
						else {
							LogInfo("Syntax error while analyzing SQL : " + whereStr + ".", 16);
							return nullptr;
						}
					}
					else if (op == "AND") {
						if (a == "FALSE" || b == "FALSE") {
							ans = "FALSE";
						}
						else if (a == "TRUE") {
							ans = b;
						}
						else if (b == "TRUE") {
							ans = a;
						}
						else {
							LogInfo("Syntax error while analyzing SQL : " + whereStr + ".", 16);
							return nullptr;
						}
					}
					else {
						if (isdigit(a[0]) == false && a[0] != '\'') {
							if (this->FieldMap.find(a) == this->FieldMap.end()) {
								LogInfo("Can not find Field named '" + a + "'.", 9);
								return nullptr;
							}
							auto p = this->FieldMap[a];
							typeIdA = p->FieldType.id;
							beginA = p->Offset;
							lengthA = p->FieldSize;
						}
						if (isdigit(b[0]) == false && b[0] != '\'') {
							if (this->FieldMap.find(b) == this->FieldMap.end()) {
								LogInfo("Can not find Field named '" + b + "'.", 9);
								return nullptr;
							}
							auto p = this->FieldMap[b];
							typeIdB = p->FieldType.id;
							beginB = p->Offset;
							lengthB = p->FieldSize;
						}
						if (typeIdA != 0 && typeIdB != 0) {
							if (typeIdA != typeIdB) {
								LogInfo("Can not compare different type.", 14);
								return nullptr;
							}
							if (lengthA != lengthB) {
								LogInfo("Can not compare different length.", 15);
								return nullptr;
							}
						}
						if (typeIdA == 0 && typeIdB == 0) {//imm op imm
							diff = atof(a.c_str()) - atof(b.c_str());
							if (diff == 0) {
								if (op == ">=" || op == "<=" || op == "=") {
									ans = "TRUE";
								}
								else {
									ans = "FALSE";
								}
							}
							else if (diff > 0) {
								if (op == ">=" || op == ">" || op == "!=") {
									ans = "TRUE";
								}
								else {
									ans = "FALSE";
								}
							}
							else {
								if (op == "<=" || op == "<" || op == "!=") {
									ans = "TRUE";
								}
								else {
									ans = "FALSE";
								}
							}
						}
						else if (typeIdA == 0 && typeIdB != 0) {//imm op data
							if (typeIdB == FieldType_INT.id) {
								int32_t _a = atoi(a.c_str());
								int32_t _b = *(int*)(data + beginB);
								diff = _a - _b;
							}
							else if (typeIdB == FieldType_FLOAT.id) {
								double _a = atof(a.c_str());
								double _b = *(double*)(data + beginB);
								diff = _a - _b;
							}
							else if (typeIdB == FieldType_CHAR.id) {
								diff = strcmp(a.c_str(), (char*)data + beginB);
							}
							else if (typeIdB == FieldType_BYTE.id) {
								diff = memcmp(a.c_str(), data + beginB, lengthB);
							}
							else {
								LogInfo("Unknown data type.", 14);
								return nullptr;
							}

						}
						else if (typeIdA != 0 && typeIdB == 0) {//data op imm
							if (typeIdA == FieldType_INT.id) {
								int32_t _b = atoi(b.c_str());
								int32_t _a = *(int*)(data + beginA);
								diff = _a - _b;
							}
							else if (typeIdA == FieldType_FLOAT.id) {
								double _b = atof(b.c_str());
								double _a = *(double*)(data + beginA);
								diff = _a - _b;
							}
							else if (typeIdA == FieldType_CHAR.id) {
								diff = memcmp(data + beginA, b.c_str(), lengthA);
							}
							else if (typeIdA == FieldType_BYTE.id) {
								diff = memcmp(data + beginA, b.c_str(), lengthA);
							}
							else {
								LogInfo("Unknown data type.", 14);
								return nullptr;
							}

						}
						else if (typeIdA != 0 && typeIdB != 0) {//data op data
							if (typeIdB == FieldType_INT.id) {
								int32_t _a = *(int*)(data + beginA);
								int32_t _b = *(int*)(data + beginB);
								diff = _a - _b;
							}
							else if (typeIdB == FieldType_FLOAT.id) {
								double _a = *(double*)(data + beginA);
								double _b = *(double*)(data + lengthB);
								diff = _a - _b;
							}
							else if (typeIdB == FieldType_CHAR.id) {
								diff = memcmp(data + beginA, data + beginB, lengthA);
							}
							else if (typeIdB == FieldType_BYTE.id) {
								diff = memcmp(data + beginA, data + beginB, lengthA);
							}
							else {
								LogInfo("Unknown data type.", 14);
								return nullptr;
							}
						}

						if (diff == 0) {
							if (op == ">=" || op == "<=" || op == "=") {
								ans = "TRUE";
							}
							else {
								ans = "FALSE";
							}
						}
						else if (diff < 0) {
							if (op == ">=" || op == ">" || op == "!=") {
								ans = "TRUE";
							}
							else {
								ans = "FALSE";
							}
						}
						else {
							if (op == "<=" || op == "<" || op == "!=") {
								ans = "TRUE";
							}
							else {
								ans = "FALSE";
							}
						}
					}
					res.push(ans);
				}
				else {
					res.push(postFix[i]);
				}
			}
			if (res.size() == 1) {
				if (res.top() == "TRUE") {
					result->push_back(index);
				}
				else if (res.top() != "FALSE") {
					LogInfo("Syntax error.", 15);
					return nullptr;
				}
				res.pop();
			}
			else {
				LogInfo("Syntax error.", 15);
				return nullptr;
			}

		}

		return result;
	}

	const string Print() {
		stringstream res;
		res << "========================================================" << "\r\n";
		res << "[Table Name : " << this->TableName << "]" << "\r\n";
		for (uint32_t i = 0; this->TableField[i] != 0; i++) {
			res << setw(15) << this->TableField[i]->FieldName;
		}
		res << "\r\n";
		for (auto iter = this->Data.begin(); iter != this->Data.end(); iter++)
		{
			for (uint32_t i = 0; i < this->TableFieldNum; i++) {
				if (this->TableField[i]->FieldSize >= 255 || this->TableField[i]->FieldType.id == FieldType_BYTE.id) {
					res << setw(15) << "***";
				}
				else {
					uint8_t buff[256];
					memset(buff, 0, 256 * sizeof(uint8_t));
					uint32_t fieldTypeID = this->TableField[i]->FieldType.id;
					memcpy(buff, *iter + this->TableField[i]->Offset, this->TableField[i]->FieldSize);
					if (fieldTypeID == FieldType_CHAR.id)
					{
						res << setw(15) << (char*)buff;
					}
					else if (fieldTypeID == FieldType_INT.id) {
						res << setw(15) << *(int*)buff;

					}
					else if (fieldTypeID == FieldType_FLOAT.id) {
						res << setw(15) << *(double*)buff;

					}
					else if (fieldTypeID == FieldType_CHAR.id) {
						res << setw(15) << buff;
					}
				}
			}
			res << "\r\n";
		}
		res << "========================================================" << "\r\n";
		return res.str();
	}
};

/// <summary>
/// 向表中插入数据
/// </summary>
/// <param name="pTable">待插入的表</param>
/// <param name="fieldNameList">列名（vector）</param>
/// <param name="valuesList">值（vector）</param>
/// <returns>插入后的表</returns>
Table* Insert(Table* pTable, const vector<string> fieldNameList, const vector<string> valuesList) {
	if (valuesList.size() != fieldNameList.size()) {
		LogInfo("Field number and Values number not match.", 8);
		return nullptr;
	}
	uint8_t* buff = (uint8_t*)calloc(1, pTable->TableRowSize);
	if (buff == NULL) {
		LogInfo("Unable to allocate memory", 12);
		return nullptr;
	}
	memset(buff, 0, pTable->TableRowSize);

	for (uint32_t i = 0; i < fieldNameList.size(); ++i) {
		auto iter = pTable->FieldMap.find(fieldNameList[i]);
		if (iter == pTable->FieldMap.end()) {
			LogInfo("Can not find such Field", 9);
			return nullptr;
		}
		auto type = iter->second->FieldType.id;
		auto offset = iter->second->Offset;
		auto length = iter->second->FieldSize;
		bool isPK = iter->second->FieldProperty & FIELD_PROPERTY_PK;

		if (isPK == true) {
			for (uint64_t j = 0; j < pTable->Data.size(); j++)
			{
				int32_t diff = 1;
				if (type == FieldType_INT.id) {
					diff = atoi(valuesList[i].c_str()) - atoi((char*)pTable->Data[j] + offset);
				}
				else if (type == FieldType_FLOAT.id)
				{
					double value = atof(valuesList[i].c_str()) - atof((char*)pTable->Data[j] + offset);
					diff = (value != 0);
				}
				else if (type == FieldType_CHAR.id || type == FieldType_BYTE.id) {
					diff = memcmp(pTable->Data[j] + offset, (uint8_t*)valuesList[i].c_str(), length);
				}
				else {
					LogInfo("Field type unknown.", 15);
					return nullptr;
				}
				if (diff == 0) {
					LogInfo("Primary key duplicate", 3);
					return nullptr;
				}
			}
		}

		if (type == FieldType_INT.id) {
			int value = atoi(valuesList[i].c_str());
			memcpy(buff + offset, &value, length);
		}
		else if (type == FieldType_FLOAT.id)
		{
			double value = atof(valuesList[i].c_str());
			memcpy(buff + offset, &value, length);
		}
		else if (type == FieldType_CHAR.id || type == FieldType_BYTE.id) {
			memcpy(buff + offset, (uint8_t*)valuesList[i].c_str(), length);
		}
		else {
			LogInfo("Field type unknown.", 15);
			return nullptr;
		}
	}
	pTable->InsertData(buff);
	free(buff);
	return pTable;
}

/// <summary>
/// 笛卡尔积
/// </summary>
/// <param name="tableNum">参与运算的表数量</param>
/// <param name="tables">参与运算的表指针</param>
/// <returns>笛卡尔积结果</returns>
Table* From(DB& pDatabase, const vector<string>& tableNameList) {
	for (size_t i = 0; i < tableNameList.size(); i++)
	{
		if (pDatabase.find(tableNameList[i]) == pDatabase.end()) {
			LogInfo("Can not find such Table", 9);
			return nullptr;
		}
	}
	if (tableNameList.size() == 0) {
		return nullptr;
	}
	Table* retTable = new Table();
	if (retTable == NULL) {
		LogInfo("Unable to allocate memory", 12);
		return nullptr;
	}
	//Table[0]
	const Table* firstTable = pDatabase[tableNameList[0]];
	retTable->TableName = firstTable->TableName;
	//Field Merge
	for (uint32_t i = 0; i < firstTable->TableFieldNum; ++i) {
		auto FieldName = firstTable->TableName + "." + firstTable->TableField[i]->FieldName;
		auto FieldProperty = firstTable->TableField[i]->FieldProperty;
		auto FieldType = firstTable->TableField[i]->FieldType;
		auto FieldSize = firstTable->TableField[i]->FieldSize / FieldType.size;
		retTable->AddField(new Field(FieldName, FieldType, FieldSize, FieldProperty));
	}
	//Data Merge
	for (auto iter = firstTable->Data.begin(); iter != firstTable->Data.end(); iter++) {
		retTable->InsertData(*iter);
	}
	//Table [1:]
	for (uint8_t i = 1; i < tableNameList.size(); ++i) {
		const Table* tableBuffer = pDatabase[tableNameList[i]];
		retTable->TableName += ("&&");
		retTable->TableName.append(tableBuffer->TableName);
		//Data Merge
		vector<uint8_t*> dataRows = vector<uint8_t*>();
		uint32_t datasize = retTable->TableRowSize + tableBuffer->TableRowSize;


		for (uint32_t i = 0; i < retTable->Data.size(); ++i) {
			for (uint32_t j = 0; j < tableBuffer->Data.size(); ++j) {
				uint8_t* mergeData = (uint8_t*)calloc(datasize, sizeof(uint8_t));
				if (mergeData == NULL) {
					LogInfo("Unable to allocate memory", 12);
					return nullptr;
				}
				memcpy(mergeData, retTable->Data[i], retTable->TableRowSize);
				memcpy(mergeData + retTable->TableRowSize, tableBuffer->Data[j], tableBuffer->TableRowSize);
				dataRows.push_back(mergeData);
			}
			free(retTable->Data[i]);
		}
		retTable->Data = dataRows;
		//Field Merge
		for (uint32_t i = 0; i < tableBuffer->TableFieldNum; ++i) {
			Field* f = (Field*)calloc(1, sizeof(Field));
			if (f == NULL) {
				LogInfo("Unable to allocate memory", 12);
				return nullptr;
			}
			f->FieldName = tableBuffer->TableName + "." + tableBuffer->TableField[i]->FieldName;
			f->FieldProperty = tableBuffer->TableField[i]->FieldProperty;
			f->FieldSize = tableBuffer->TableField[i]->FieldSize;
			f->FieldType = tableBuffer->TableField[i]->FieldType;
			retTable->AddField(f);
		}

	}
	return retTable;
}

/// <summary>
/// 通过`constraint`条件筛选行
/// </summary>
/// <param name="constraint">约束条件</param>
/// <returns>查询结果</returns>
Table* Where(Table* pTable, const vector<uint64_t>& index) {
	Table* retTable = new Table();
	retTable->TableName = pTable->TableName + "'";
	for (uint32_t i = 0; i < pTable->TableFieldNum; ++i) {
		Field* newField = new Field(pTable->TableField[i]);
		retTable->AddField(newField);
	}
	for (uint64_t i = 0; i < index.size(); ++i) {
		retTable->InsertData(pTable->Data[index[i]]);
	}
	return retTable;
}

/// <summary>
/// 通过`constraint`条件删除行
/// </summary>
/// <param name="constraint">约束条件</param>
/// <returns>删除结果</returns>
Table* Delete(Table* pTable, const vector<uint64_t>& index) {
	for (int64_t i = index.size() - 1; i >= 0; --i) {
		pTable->DeleteData(index[i]);
	}
	return pTable;
}

/// <summary>
/// 选择表中若干列
/// </summary>
/// <param name="fieldNames">属性名列表</param>
/// <returns>投影后的结果</returns>
Table* Select(const Table* pTable, const vector<string> fieldNames) {
	Table* retTable = new Table();
	retTable->TableName = pTable->TableName + "'";
	vector<uint32_t> begin, length;

	for (uint32_t i = 0; i < fieldNames.size(); ++i) {
		if (fieldNames[i] == "*") {
			begin.push_back(0);
			length.push_back(pTable->TableRowSize);
			for (uint32_t i = 0; i < pTable->TableFieldNum; ++i) {
				auto pField = new Field(pTable->TableField[i]);
				retTable->AddField(pField);
				continue;
			}
		}
		for (uint32_t j = 0; j < pTable->TableFieldNum; ++j) {
			if (fieldNames[i] == pTable->TableField[j]->FieldName) {
				begin.push_back(pTable->TableField[j]->Offset);
				length.push_back(pTable->TableField[j]->FieldSize);

				auto pField = new Field(pTable->TableField[j]);
				retTable->AddField(pField);
				break;
			}
		}
	}
	uint8_t* buff = (uint8_t*)malloc(retTable->TableRowSize);
	if (buff == NULL) {
		LogInfo("Unable to allocate memory", 12);
		return nullptr;
	}
	uint32_t offset;
	for (uint64_t row = 0; row < pTable->Data.size(); ++row) {
		offset = 0;
		for (uint32_t f = 0; f < begin.size(); ++f) {
			memcpy(buff + offset, pTable->Data[row] + begin[f], length[f]);
			offset += length[f];
		}
		retTable->InsertData(buff);
	}
	free(buff);
	return retTable;
}

Table* Update(Table* pTable, const vector<string>& Fields, const vector<string>& Datas, const vector<uint64_t>& index) {
	vector<uint8_t*> pData;
	vector<uint32_t> offset, length;
	for (uint32_t i = 0; i < Fields.size(); ++i) {
		if (pTable->FieldMap.find(Fields[i]) == pTable->FieldMap.end()) {
			LogInfo("Can not find such Field '" + Fields[i] + "'", 9);
			return nullptr;
		}
		auto f = pTable->FieldMap[Fields[i]];
		auto size = f->FieldSize;
		offset.push_back(f->Offset);
		length.push_back(size);
		uint8_t* buff = (uint8_t*)malloc(size);

		auto typeId = f->FieldType.id;
		if (typeId == FieldType_INT.id) {
			auto a = atoi(Datas[i].c_str());
			memcpy(buff, &a, size);
		}
		else if (typeId == FieldType_FLOAT.id) {
			auto a = atof(Datas[i].c_str());
			memcpy(buff, &a, size);
		}
		else if (typeId == FieldType_CHAR.id) {
			if (Datas[i].length() < size) {
				strcpy((char*)buff, Datas[i].c_str());
			}
			else {
				memcpy(buff, Datas[i].c_str(), size);//截断
			}
		}
		else if (typeId == FieldType_BYTE.id) {
			memcpy(buff, Datas[i].c_str(), size);
		}
		else {
			LogInfo("Unknown data type.", 14);
			return nullptr;
		}
		pData.push_back(buff);
	}
	for (uint32_t i = 0; i < index.size(); i++)
	{
		for (uint32_t j = 0; j < pData.size(); j++)
		{
			pTable->UpdateData(pData[j], offset[j], length[j], index[i]);
		}
	}
	return pTable;
}

Table* Create(DB& pDatabase, const string& tableName, const vector<tuple<string, string, int, int>>& Fields) {
	auto FieldNum = Fields.size();
	if (FieldNum > MAX_FIELD_NUMBER) {
		LogInfo("Field number exceed", 12);
		return nullptr;
	}

	Table* pTable = new Table(tableName);
	for (uint32_t i = 0; i < FieldNum; i++)
	{
		auto type = std::get<1>(Fields[i]);
		const Datatype* ptype = nullptr;
		if (type == "INT") {
			ptype = &FieldType_INT;
		}
		else if (type == "FLOAT") {
			ptype = &FieldType_FLOAT;

		}
		else if (type == "CHAR")
		{
			ptype = &FieldType_CHAR;
		}
		else if (type == "BYTE")
		{
			ptype = &FieldType_BYTE;
		}
		else
		{
			LogInfo("Unknown data type.", 14);
			return nullptr;
		}
		Field* pField = new Field(std::get<0>(Fields[i]), *ptype, std::get<2>(Fields[i]), std::get<3>(Fields[i]));
		pTable->AddField(pField);
	}
	pDatabase[pTable->TableName] = pTable;
	return pTable;
}

void Drop(DB& pDatabase, const string& tableName) {
	auto iter = pDatabase.find(tableName);
	if (iter == pDatabase.end()) {
		LogInfo("Can not find such Table", 9);
		return;
	}
	auto pTable = iter->second;
	delete pTable;
	pDatabase.erase(iter);
	return;
}

void StoreDatabase(string filePath, const DB& database) {
	ofstream fs;
	fs.open(filePath, ios::out | ios::binary);
	auto tableend = database.end();
	for (auto iter = database.begin(); iter != tableend; ++iter)
	{
		const Table* table = iter->second;
		uint8_t* buff = (uint8_t*)calloc(1, TABLE_HEAD_SIZE);
		if (buff == NULL) {
			LogInfo("Unable to allocate memory", 12);
			return;
		}
		uint64_t rowNumber = table->Data.size();
		//写入表名
		strcpy((char*)buff, table->TableName.c_str());
		//属性数量uint32_t
		memcpy(buff + MAX_TABLE_NAME_LEN, &table->TableFieldNum, sizeof(table->TableFieldNum));
		//行数uint64_t
		memcpy(buff + MAX_TABLE_NAME_LEN + sizeof(table->TableFieldNum), &rowNumber, sizeof(rowNumber));
		//数据长度uint32_t
		memcpy(buff + MAX_TABLE_NAME_LEN + sizeof(table->TableFieldNum) + sizeof(rowNumber), &table->TableRowSize, sizeof(table->TableRowSize));
		fs.write((const char*)buff, TABLE_HEAD_SIZE);
		free(buff);
		for (uint32_t i = 0; i < table->TableFieldNum; i++)
		{
			const Field* field = table->TableField[i];
			uint8_t* buff = (uint8_t*)calloc(1, FIELD_SIZE);
			if (buff == NULL) {
				LogInfo("Unable to allocate memory", 12);
				return;
			}
			//写入属性名
			strcpy((char*)buff, field->FieldName.c_str());
			//写入类型号uint8_t
			memcpy(buff + MAX_FIELD_NAME_LEN, &field->FieldType.id, sizeof(field->FieldType.id));
			//写入长度uint32_t
			memcpy(buff + MAX_FIELD_NAME_LEN + sizeof(field->FieldType.id), &field->FieldSize, sizeof(field->FieldSize));
			//写入标签uint32_t
			memcpy(buff + MAX_FIELD_NAME_LEN + sizeof(field->FieldType.id) + sizeof(field->FieldSize), &field->FieldProperty, sizeof(field->FieldProperty));
			fs.write((const char*)buff, FIELD_SIZE);
			free(buff);
		}
		uint32_t rowSize = table->TableRowSize;
		for (uint64_t i = 0; i < rowNumber; i++)
		{
			fs.write((const char*)table->Data[i], rowSize);
		}
	}
	fs.close();
}

void LoadDatabase(string filePath, DB& database) {
	for (auto iter = database.begin(); iter != database.end(); ++iter) {
		delete iter->second;
	}
	database.clear();
	ifstream fs;
	fs.open(filePath, ios::in | ios::binary);
	while (fs.peek() != EOF)
	{
		Table* pTable = new Table();
		char pTableName[MAX_TABLE_NAME_LEN];
		uint32_t TableFieldNum;
		uint64_t RowNumber;
		uint32_t RowSize;
		//读取表名
		fs.read(pTableName, MAX_TABLE_NAME_LEN);
		pTable->TableName = pTableName;
		//属性数量uint32_t
		fs.read((char*)&TableFieldNum, sizeof(pTable->TableFieldNum));
		//行数uint64_t
		fs.read((char*)&RowNumber, sizeof(RowNumber));
		//数据长度uint32_t
		fs.read((char*)&RowSize, sizeof(RowSize));
		for (uint32_t i = 0; i < TableFieldNum; i++)
		{
			char FieldName[MAX_FIELD_NAME_LEN];
			uint8_t type = 0;
			uint32_t FieldSize = 0;
			uint32_t prop = 0;
			const Datatype* datatype = nullptr;
			//属性名
			fs.read(FieldName, MAX_FIELD_NAME_LEN);
			//类型号uint8_t
			fs.read((char*)&type, sizeof(uint8_t));
			if (type == FieldType_INT.id) {
				datatype = &FieldType_INT;
			}
			else if (type == FieldType_FLOAT.id) {
				datatype = &FieldType_FLOAT;
			}
			else if (type == FieldType_CHAR.id) {
				datatype = &FieldType_CHAR;
			}
			else if (type == FieldType_BYTE.id) {
				datatype = &FieldType_BYTE;
			}
			else {
				LogInfo("Unknown data type.", 14);
				return;
			}
			//长度uint32_t
			fs.read((char*)&FieldSize, sizeof(uint32_t));
			//标签uint32_t
			fs.read((char*)&prop, sizeof(uint32_t));

			Field* pField = new Field(FieldName, *datatype, FieldSize / datatype->size, prop);
			pTable->AddField(pField);
		}
		assert(RowSize == pTable->TableRowSize);
		uint8_t* buff = (uint8_t*)malloc(RowSize);
		for (uint64_t i = 0; i < RowNumber; i++)
		{
			fs.read((char*)buff, RowSize);
			pTable->InsertData(buff);
		}
		free(buff);
		(database)[pTable->TableName] = pTable;
	}
}

string SQL(DB& db, string sql) {
	if (sql == "") {
		return "";
	}
	if (sql[sql.length() - 1] != ';') {
		LogInfo("Missing semicolon at the end of SQL", 5);
		sql = sql + ";";
	}
	string upperSql = sql;
	transform(upperSql.begin(), upperSql.end(), upperSql.begin(), ::toupper);

	if (upperSql.compare(0, 5, "SELECT", 0, 5) == 0) {
		auto selectIndex = upperSql.find("SELECT");//selectIndex+7
		auto fromIndex = upperSql.find("FROM");//fromIndex+5
		auto whereIndex = upperSql.find("WHERE");//whereIndex+6

		string whereStr;
		if (whereIndex == string::npos) {
			whereIndex = upperSql.length() - 1;
			whereStr = "";
		}
		else {
			whereStr = sql.substr(whereIndex + 6, sql.length() - whereIndex - 7);

		}
		string select = sql.substr(selectIndex + 7, fromIndex - selectIndex - 7 - 1);
		string from = strip(sql.substr(fromIndex + 5, whereIndex - fromIndex - 5));

		vector<string> selectVec = split(select, ",");
		vector<string> fromVec = split(from, ",");
		Table* fromTable = From(db, fromVec);
		if (fromTable == nullptr) {
			return "Table " + from + " Not found.";
		}
		auto rowList = fromTable->WhereFilter(strip(whereStr));
		if (rowList == nullptr) {
			return "Syntax error around where";
		}
		Table* whereTable = Where(fromTable, *rowList);
		if (whereTable == nullptr) {
			return "Syntax error around where ";
		}
		Table* selectTable = Select(whereTable, selectVec);
		if (whereTable == nullptr) {
			free(whereTable);
			return "Syntax error around select ";
		}
		string result = (selectTable->Print());
		delete (whereTable);
		delete (rowList);
		delete (fromTable);
		delete (selectTable);
		return result;
	}
	else if (upperSql.compare(0, 11, "CREATE TABLE", 0, 11) == 0) {
		auto begin = sql.find("(");
		auto end = sql.rfind(")");
		string tableNamestr = sql.substr(13, begin - 12 - 2);
		Table* pTable = new Table(tableNamestr);
		string fieldstr = sql.substr(begin + 1, end - begin - 1);
		auto fields = split(fieldstr, ",");
		for (uint32_t i = 0; i < fields.size(); ++i) {
			uint32_t size = 0;

			auto UpperField = fields[i];
			transform(UpperField.begin(), UpperField.end(), UpperField.begin(), ::toupper);

			auto sizebegin = fields[i].find("(");
			auto sizeend = fields[i].find(")");
			if (sizebegin == string::npos) {
				size = 1;
			}
			else {
				size = atoi(fields[i].substr(sizebegin + 1, sizeend - sizebegin - 1).c_str());
				fields[i].erase(sizebegin, sizeend - sizebegin + 1);
			}
			auto fieldInfo = split(fields[i], " ");
			transform(fieldInfo[1].begin(), fieldInfo[1].end(), fieldInfo[1].begin(), ::toupper);
			const Datatype* pType = nullptr;
			if (fieldInfo[1] == "INT") {
				pType = &FieldType_INT;
			}
			else if (fieldInfo[1] == "FLOAT") {
				pType = &FieldType_FLOAT;
			}
			else if (fieldInfo[1] == "CHAR") {
				pType = &FieldType_CHAR;
			}
			else if (fieldInfo[1] == "BYTE") {
				pType = &FieldType_BYTE;
			}
			else {
				LogInfo("Unknown data type.", 14);
				return "Unknown data type.";
			}
			uint32_t fieldProp = FIELD_PROPERTY_DEFAULT;
			if (UpperField.find("PRIMARY KEY") != string::npos) {
				fieldProp = fieldProp | FIELD_PROPERTY_PK;
			}
			if (UpperField.find("NOT NULL") != string::npos) {
				fieldProp = fieldProp | FIELD_PROPERTY_NOT_NULL;
			}

			Field* pField = new Field(fieldInfo[0], *pType, size, fieldProp);
			pTable->AddField(pField);
		}
		db[pTable->TableName] = pTable;
		return pTable->Print();
	}
	else if (upperSql.compare(0, 10, "INSERT INTO", 0, 10) == 0) {
		auto sqlSplite = split(sql, " ");
		string tableName = sqlSplite[2];
		auto FirstLeftParenthese = sql.find_first_of("(");
		auto LastLeftParenthese = sql.find_last_of("(");
		auto FirstRightParenthese = sql.find_first_of(")");
		auto LastRightParenthese = sql.find_last_of(")");
		if (FirstLeftParenthese == LastLeftParenthese) {//全插入
			string datastr = sql.substr(LastLeftParenthese + 1, LastRightParenthese - LastLeftParenthese - 1);
			Table* pTable = db[tableName];
			vector<string> allField;
			for (uint32_t i = 0; i < pTable->TableFieldNum; i++)
			{
				allField.push_back(pTable->TableField[i]->FieldName);
			}
			Insert(db[tableName], allField, split(datastr, ","));
		}
		else {//指定插入
			string fieldstr = sql.substr(FirstLeftParenthese + 1, FirstRightParenthese - FirstLeftParenthese - 1);
			string datastr = sql.substr(LastLeftParenthese + 1, LastRightParenthese - LastLeftParenthese - 1);
			Insert(db[tableName], split(fieldstr, ","), split(datastr, ","));
		}
		return db[tableName]->Print();

	}
	else if (upperSql.compare(0, 9, "DROP TABLE", 0, 9) == 0) {
		string tableName = sql.substr(10 + 1, sql.length() - 10 - 2);
		Drop(db, tableName);
		return tableName + "Drop\n";

	}
	else if (upperSql.compare(0, 10, "DELETE FROM", 0, 10) == 0) {
		auto fromIndex = upperSql.find("FROM");//fromIndex+5
		auto whereIndex = upperSql.find("WHERE");//whereIndex+6

		string whereStr;
		if (whereIndex == string::npos) {
			whereIndex = upperSql.length() - 1;
			whereStr = "";
		}
		else {
			whereStr = sql.substr(whereIndex + 6, sql.length() - whereIndex - 7);
		}
		string from = strip(sql.substr(fromIndex + 5, whereIndex - fromIndex - 5));
		Table* fromTable = From(db, { from });
		if (fromTable == nullptr) {
			return "Syntax error around from";
		}
		auto rowList = fromTable->WhereFilter(whereStr);
		if (rowList == nullptr) {
			return "Syntax error around where";
		}
		Delete(db[from], *rowList);
		free(fromTable);
		free(rowList);
		return db[from]->Print();
	}
	else if (upperSql.compare(0, 5, "UPDATE", 0, 5) == 0) {
		auto setIndex = upperSql.find("SET");
		auto whereIndex = upperSql.find("WHERE");
		string whereStr;
		if (whereIndex == string::npos) {
			whereIndex = upperSql.length() - 1;
			whereStr = "";
		}
		else {
			whereStr = sql.substr(whereIndex + 6, sql.length() - whereIndex - 7);
		}
		string tableName = sql.substr(7, setIndex - 7 - 1);
		if (db.find(tableName) == db.end()) {
			LogInfo("Table named '" + tableName + "' not found.", 15);
			return "Table named '" + tableName + "' not found.";
		}
		auto KeyValue = split(strip(sql.substr(setIndex + 3, whereIndex - setIndex - 3)), ",");
		auto rowList = db[tableName]->WhereFilter(whereStr);
		if (rowList == nullptr) {
			return "Syntax error around where";
		}
		vector<string> key, value;
		for (uint32_t i = 0; i < KeyValue.size(); i++)
		{
			key.push_back(strip(split(KeyValue[i], "=")[0]));
			value.push_back(strip(split(KeyValue[i], "=")[1]));
		}

		Update(db[tableName], key, value, *rowList);
		delete rowList;
		return db[tableName]->Print();

	}
	else if (upperSql.compare(0, 3, "LOAD", 0, 3) == 0) {
		string filePath = strip(split(sql.substr(0, sql.length() - 1), " ")[1]);
		LoadDatabase(filePath, db);
		string tableList;
		for (auto i = db.begin(); i != db.end(); ++i) {
			tableList += i->second->TableName + " ";
		}
		return "Load form " + filePath + "\r\nTable list : " + tableList;
	}
	else if (upperSql.compare(0, 4, "STORE", 0, 4) == 0) {
		string filePath = strip(split(sql.substr(0, sql.length() - 1), " ")[1]);
		StoreDatabase(filePath, db);
		return "Store to " + filePath + "\n";
	}
	return "Unknown SQL.";
}

int main() {
	DB database = DB();

	LoadDatabase("TestDataBase.hex", database);
	SQL(database, "CREATE TABLE test1 (a int PRIMARY KEY,b char(1020));");
	for (int i = 0; i < 100; ++i) {
		SQL(database, "INSERT INTO test1 VALUES (" + to_string(i) + ",'test');");
	}


	while (true)
	{
		string input;
		getline(cin, input);
		auto begin = clock();
		auto ans = SQL(database, input);
		auto end = clock();

		cout << ans << endl;
		cout << "Exec " << end - begin << " ms" << endl;

	}

	return 0;


	_CrtDumpMemoryLeaks();

}

#define _CRT_SECURE_NO_WARNINGS

#include "./DataStructureModule.h"
using namespace std;

class ExchangeData;
class Table;
class Field;
// https://stackoverflow.com/questions/4266914/how-does-a-const-struct-differ-from-a-struct
// 根据这个链接，const 可以删
struct Datatype
{
	uint8_t id;
	uint32_t size;
};
const Datatype INT = { 1, 4 };
const Datatype FLOAT = { 3, 8 };
const Datatype CHAR = { 10, 1 };
const Datatype BYTE = { 11, 1 };

typedef map<string, Table*> DB;

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

/// <summary>
/// 数据交换格式（pData是引用）
/// </summary>
class ExchangeData
{
public:
	struct exData
	{
		uint8_t* pData;
		uint32_t length;
	};
	map<string, ExchangeData::exData*>DataTuple;
	~ExchangeData()
	{
		auto iter = this->DataTuple.begin();
		for (iter = this->DataTuple.begin(); iter != this->DataTuple.end(); iter++) {
			free(iter->second);
		}
		this->DataTuple.clear();
	}
	uint8_t* Data(string key) {
		return this->DataTuple[key]->pData;
	}
	uint32_t Length(string key) {
		return this->DataTuple[key]->length;
	}
};

//属性名+类型号+长度+标签
#define FIELD_SIZE (MAX_FIELD_NAME_LEN+sizeof(uint8_t)+sizeof(uint32_t)+sizeof(uint32_t))
class Field
{
public:
	Field(char* fieldName, Datatype fieldType, uint32_t length, uint32_t FieldProperty = 0) {
		this->FieldName = fieldName;
		this->FieldType = fieldType;
		if (fieldType.id == CHAR.id || fieldType.id == BYTE.id) {
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
	Datatype FieldType = INT;
	uint32_t FieldProperty = 0;
	uint32_t FieldSize = 1;
	uint32_t Offset = 0;
};

//表名+属性数量+数据数量+数据长度
#define TABLE_HEAD_SIZE (MAX_TABLE_NAME_LEN+sizeof(uint32_t)+sizeof(uint64_t)+sizeof(uint32_t))

class Table
{

public:
	ExchangeData* ExData_Cache = nullptr;
	void initCache() {
		delete ExData_Cache;
		this->ExData_Cache = new ExchangeData();
		for (uint32_t i = 0; i < this->TableFieldNum; ++i) {
			string fieldName = this->TableField[i]->FieldName;
			ExchangeData::exData* exData = (ExchangeData::exData*)malloc(sizeof(ExchangeData::exData));
			if (exData == NULL) {
				LogInfo("Unable to allocate memory", 12);
				return;
			}
			ExData_Cache->DataTuple[fieldName] = exData;
		}
	}

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
	Table(char* tableName) {
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
			this->initCache();
			FieldMap[f->FieldName] = f;
			return;
		}
	}
	/// <summary>
	/// 将表中的第`index`行转化为交换格式
	/// </summary>
	/// <param name="index">待转化的行</param>
	/// <returns>交换格式</returns>
	ExchangeData* MakeExchangeData(uint64_t index) {
		if (index >= this->Data.size()) {
			LogInfo("Index out of range.", 8);
			return nullptr;
		}

		for (uint32_t i = 0; i < this->TableFieldNum; ++i) {
			string fName = this->TableField[i]->FieldName;
			this->ExData_Cache->DataTuple[fName]->length = this->TableField[i]->FieldSize;
			this->ExData_Cache->DataTuple[fName]->pData = this->Data[index] + this->TableField[i]->Offset;
		}
		return ExData_Cache;
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

	const void Print() {
		printf("========================================================\n");
		for (uint32_t i = 0; this->TableField[i] != 0; i++) {
			printf("%-15s", this->TableField[i]->FieldName.c_str());
		}
		printf("\n");
		for (auto iter = this->Data.begin(); iter != this->Data.end(); iter++)
		{
			for (uint32_t i = 0; i < this->TableFieldNum; i++) {
				if (this->TableField[i]->FieldSize >= 255 || this->TableField[i]->FieldType.id == BYTE.id) {
					printf("***\t");
				}
				else {
					uint8_t buff[256];
					memset(buff, 0, 256 * sizeof(uint8_t));
					uint32_t fieldTypeID = this->TableField[i]->FieldType.id;
					memcpy(buff, *iter + this->TableField[i]->Offset, this->TableField[i]->FieldSize);
					if (fieldTypeID == CHAR.id)
					{
						printf("%-15s", (char*)buff);
					}
					else if (fieldTypeID == INT.id) {
						printf("%-15d", *(int*)buff);
					}
					else if (fieldTypeID == FLOAT.id) {
						printf("%-15lf", *(double*)buff);
					}
					else if (fieldTypeID == CHAR.id) {
						printf("%-15s\t\t", buff);
					}
				}
			}
			printf("\n");
		}
		printf("========================================================\n");

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
		if (type == INT.id) {
			int value = atoi(valuesList[i].c_str());
			memcpy(buff + offset, &value, length);
		}
		else if (type == FLOAT.id)
		{
			double value = atof(valuesList[i].c_str());
			memcpy(buff + offset, &value, length);
		}
		else if (type == CHAR.id || type == BYTE.id) {
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
Table* From(DB& pDatabase, vector<string> tableNameList) {
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
	retTable->TableName.assign(firstTable->TableName);
	//Field Merge
	for (uint32_t i = 0; i < firstTable->TableFieldNum; ++i) {
		Field* f = (Field*)calloc(1, sizeof(Field));
		if (f == NULL) {
			LogInfo("Unable to allocate memory", 12);
			return nullptr;
		}

		f->FieldName = firstTable->TableName + "." + firstTable->TableField[i]->FieldName;
		f->FieldProperty = firstTable->TableField[i]->FieldProperty;
		f->FieldSize = firstTable->TableField[i]->FieldSize;
		f->FieldType = firstTable->TableField[i]->FieldType;
		retTable->AddField(f);
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
Table* Where(Table* pTable, bool(*constraint)(ExchangeData* pExData)) {
	//TODO this pTable has no name
	Table* retTable = new Table();
	for (uint32_t i = 0; i < pTable->TableFieldNum; ++i) {
		Field* newField = new Field(pTable->TableField[i]);
		retTable->AddField(newField);
	}
	for (uint64_t i = 0; i < pTable->Data.size(); ++i) {
		pTable->MakeExchangeData(i);
		if (constraint(pTable->ExData_Cache) == true) {
			retTable->InsertData(pTable->Data[i]);
		}
	}
	return retTable;
}

/// <summary>
/// 通过`constraint`条件删除行
/// </summary>
/// <param name="constraint">约束条件</param>
/// <returns>删除结果</returns>
Table* Delete(Table* pTable, bool(*constraint)(ExchangeData* pExData)) {
	for (uint32_t i = 0; i < pTable->TableFieldNum; ++i) {
		Field* newField = new Field(pTable->TableField[i]);
		pTable->AddField(newField);
	}
	for (uint64_t i = 0; i < pTable->Data.size(); ++i) {
		pTable->MakeExchangeData(i);
		if (constraint(pTable->ExData_Cache) == true) {
			pTable->DeleteData(i);
		}
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
	vector<uint32_t> begin, length;

	for (uint32_t i = 0; i < fieldNames.size(); ++i) {
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

void StoreDatabase(string filePath, DB* database) {
	ofstream fs;
	fs.open(filePath, ios::out | ios::binary);
	auto tableend = database->end();
	for (auto iter = database->begin(); iter != tableend; ++iter)
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

void LoadDatabase(string filePath, DB* database) {
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
			if (type == INT.id) {
				datatype = &INT;
			}
			else if (type == FLOAT.id) {
				datatype = &FLOAT;
			}
			else if (type == CHAR.id) {
				datatype = &CHAR;
			}
			else {
				datatype = &BYTE;
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
		(*database)[pTable->TableName] = pTable;
	}
}

bool __connect_on_no(ExchangeData* ex) {
	if (*(int*)(ex->Data("Students.No")) == *(int*)(ex->Data("Score.No"))) {
		return true;
	}
	return false;
}

int main() {

	DB database = DB();

	Table* studentTable = new Table((char*)"Students");

	studentTable->AddField(new Field((char*)"Name", CHAR, 32, FIELD_PROPERTY_DEFAULT));
	studentTable->AddField(new Field((char*)"No", INT, 1, FIELD_PROPERTY_PK | FIELD_PROPERTY_INDEX));
	studentTable->AddField(new Field((char*)"Age", INT, 1, FIELD_PROPERTY_DEFAULT));
	studentTable->AddField(new Field((char*)"Gender", CHAR, 1, FIELD_PROPERTY_DEFAULT));
	studentTable->AddField(new Field((char*)"Grade", INT, 1, FIELD_PROPERTY_DEFAULT));

	Insert(studentTable, { "Name", "No",  "Gender", "Age","Grade" }, { "Peter", "20010",  "M","19", "2" });
	Insert(studentTable, { "Name", "No", "Age", "Gender", "Grade" }, { "Azura", "20011", "20", "M", "3" });
	Insert(studentTable, { "Name", "No", "Age", "Gender", "Grade" }, { "Monika", "20012", "20", "F", "3" });
	Insert(studentTable, { "Name", "No", "Age", "Gender", "Grade" }, { "Mike", "20013", "19", "M", "2" });
	Insert(studentTable, { "Name", "No", "Age", "Gender", "Grade" }, { "Alice", "20014", "19", "F", "2" });

	database[studentTable->TableName] = studentTable;

	Table* scoreTable = new Table((char*)"Score");
	scoreTable->AddField(new Field((char*)"Point", FLOAT, 1, FIELD_PROPERTY_DEFAULT));
	scoreTable->AddField(new Field((char*)"Mark", INT, 1, FIELD_PROPERTY_DEFAULT));
	scoreTable->AddField(new Field((char*)"No", INT, 1, FIELD_PROPERTY_PK | FIELD_PROPERTY_INDEX));

	Insert(scoreTable, { "Mark", "No","Point" }, { "98", "20010","3.5" });
	Insert(scoreTable, { "Mark","Point", "No" }, { "99", "4.5", "20011" });
	Insert(scoreTable, { "Point","Mark", "No" }, { "3.7", "59", "20012" });
	Insert(scoreTable, { "Mark", "No" }, { "53", "20013" });
	Insert(scoreTable, { "Mark", "No" }, { "97", "20014" });

	database[scoreTable->TableName] = scoreTable;

	//StoreDatabase("TestDataBase.hex", &database);
	//return 0;

	DB Loaddb = DB();
	LoadDatabase("TestDataBase.hex", &Loaddb);

	auto start = clock();
	Table* crossJoin = new Table();
	Table* ls[] = { studentTable, scoreTable };

	auto times = 1;
	for (int i = 0; i < times; ++i) {
		//delete crossJoin;
		//crossJoin = From(2, ls);

		//memcpy(studentRow + studentTable->TableField[1]->Offset, &no, studentTable->TableField[1]->FieldSize);

		//studentTable->InsertData(studentRow);
		//studentTable->DeleteData(0);
	}
	auto end = clock();
	printf("Time Cost %lf ms \n", (double)(end - start) / times);

	Loaddb["Students"]->Print();
	Loaddb["Score"]->Print();

	crossJoin = From(Loaddb, { "Students","Score" });
	crossJoin->Print();
	auto Scores = Select(Where(crossJoin, __connect_on_no), { "Students.Name", "Students.No","Score.Score" });
	Scores->Print();


	//crossJoin->DeleteData(1);
	//crossJoin->Print();



	auto ss = crossJoin->MakeExchangeData(0);
	//cout << (char*)ss->Data("Students.Name") << endl;
	return 0;
}



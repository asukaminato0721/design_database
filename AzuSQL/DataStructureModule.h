#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <bits/stdc++.h>
#include "MyDBMSDlg.h"
using namespace std;


#define MAX_TABLE_NAME_LEN 128
#define MAX_FIELD_NAME_LEN 128
#define MAX_FIELD_NUMBER 64

#define FIELD_PROPERTY_DEFAULT 0
#define FIELD_PROPERTY_PK 1<<0
#define FIELD_PROPERTY_NOT_NULL 1<<4
#define FIELD_PROPERTY_INDEX 1<<8

class Table;
class Field;
typedef map<string, Table*> DB;

struct Datatype
{
	uint8_t id;
	uint32_t size;
};
const Datatype FieldType_INT = { 1, 4 };
const Datatype FieldType_FLOAT = { 3, 8 };
const Datatype FieldType_CHAR = { 10, 1 };
const Datatype FieldType_BYTE = { 11, 1 };

#define FIELD_SIZE (MAX_FIELD_NAME_LEN+sizeof(uint8_t)+sizeof(uint32_t)+sizeof(uint32_t))
class Field
{
public:
	Field(string fieldName, Datatype fieldType, uint32_t length, uint32_t FieldProperty);
	Field(Field* f);
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

	Table();
	Table(string tableName);
	~Table();
	void AddField(Field* f);
	Table* InsertData(uint8_t* data);
	Table* DeleteData(uint64_t rowIndex);
	Table* UpdateData(uint8_t* pData, uint32_t offset, uint32_t length, uint64_t rowIndex);
	vector<uint64_t>* WhereFilter(const string& whereStr);
	const string Print();
};



string strip(const string& str);
vector<string> split(const string& str, const string& delim);
string SQL(DB& db, string sql);

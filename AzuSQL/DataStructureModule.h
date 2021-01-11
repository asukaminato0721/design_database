#pragma once

#include <bits/stdc++.h>
using namespace std;
class ExchangeData;
class Table;
class Field;
typedef map<string, Table*> DB;

#define _CRT_SECURE_NO_WARNINGS

#define MAX_TABLE_NAME_LEN 128
#define MAX_FIELD_NAME_LEN 128
#define MAX_FIELD_NUMBER 64

#define FIELD_PROPERTY_DEFAULT 0
#define FIELD_PROPERTY_PK 1<<0
#define FIELD_PROPERTY_NOT_NULL 1<<4
#define FIELD_PROPERTY_INDEX 1<<8

string strip(const string& str);
vector<string> split(const string& str, const string& delim);
string SQL(DB& db, string sql);

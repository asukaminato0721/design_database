# 一些用于处理sql语句的函数
import sqlparse
from DataStructureModule import *
from sql_parse.lexical_analysis import parse_sql

reserved_words=["+","-",">","<","="]
class Parsed:
    type: str = ""
    columns: list = []
    setOrInsert_value: list = []
    tables: list = []
    wheres: str = ""


def extract_tables(table_str):
    """
    提取表的别名，输入sql语句，返回一个字典 key为表别名，value为表真名
    """
    tables = {}
    table_list = table_str.split(',')
    for item in table_list:
        if item.split(' ')[1] in tables:
            if item.split(' ')[0] != tables[item.split(' ')[1]]:
                return {}
        else:
            tables[item.split(' ')[1]] = item.split(' ')[0]
    return tables


def format_parsed(parsed:Parsed,tables:{},tables_cols:{}):
    if len(parsed.tables)==1:
        for i in range(len(parsed.columns)):
            if "." in parsed.columns[i]:
                parsed.columns[i]=list(parsed.columns[i].split("."))[1]
    else:
        for i in range(len(parsed.columns)):
            if "." in parsed.columns[i]: # 属性名已经有前缀表名
                tableName=list(parsed.columns[i].split("."))[0]
                if tableName in tables.keys():
                    tableName=tables[tableName]
            else:
                columnName=parsed.columns[i]
                for key in tables_cols.keys():
                    if columnName in tables_cols[key]:
                        parsed.columns[i]=key+"."+columnName
    for i in range(len(parsed.tables)):
        parsed.tables[i]=list(parsed.tables[i].split(" "))[0]


def getResult() -> Table:
    parsed_l = parse_sql()
    print(parsed_l)
    parsed=Parsed()
    tables_other_name={}
    result_table=Table()
    for i in range(len(parsed_l)):
        if i==0:
            parsed.type=parsed_l[0][0].lower()  #获取sql语句的类型
        else:
            if parsed.type=="select":
                parsed.columns=list(parsed_l[0][1].split(","))
                if parsed_l[i][0].lower()=="from":
                    parsed.tables=list(parsed_l[i][1].split(","))
                    tables_other_name=extract_tables(parsed_l[i][1])
                elif parsed_l[i][0].lower()=="where":
                    parsed.wheres=list(parsed_l[i][1].split(","))
    format_parsed(parsed,tables_other_name,{})
    tables_cols={}
    for table in parsed.tables:
        cols=[]
        for field in FindTable(table).TableField:
            cols.append(field.FieldName)
        tables_cols[table]=cols
    format_parsed(parsed,tables_other_name,tables_cols) #格式化
    if parsed.type=="select":
        tables=[]
        for item in parsed.tables:
            tables.append(FindTable(item))
        result_table=Select(
            Where(
                From(
                    *tables
                ),
                lambda line:eval("line[\"No\"]>10012")
            ),
            parsed.columns
        )
    return result_table

PrintTable(getResult())
# 一些用于处理sql语句的函数
import sqlparse
from DataStructureModule import *
from sql_parse.lexical_analysis import parse_sql


def extract_tables(sql):
    """
    提取表的别名，输入sql语句，返回一个字典 key为表别名，value为表真名
    """
    tables = {}
    parsed = sqlparse.parse(sql)
    query_tokens = parsed[0].tokens
    table_str=''
    found = 0
    for token in query_tokens:
        if found == 0:
            if str(token) == "from" or str(token)=="FROM":
                found = 1
        else:
            if isinstance(token,sqlparse.sql.Identifier) or isinstance(token,sqlparse.sql.IdentifierList):
                table_str=str(token)
                break
    table_list=table_str.split(',')
    for item in table_list:
        if item.split(' ')[1] in tables:
            if item.split(' ')[0]!=tables[item.split(' ')[1]]:
                return {}
        else:
            tables[item.split(' ')[1]]=item.split(' ')[0]
    return tables


PrintTable(FindTable("Student"))

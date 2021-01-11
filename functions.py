# 一些用于处理sql语句的函数
import sqlparse
from DataStructureModule import *
from sql_parse.lexical_analysis import parse_sql


class Parsed:
    type: str = ""
    columns: list = []
    tables: list = []
    wheres: str


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


PrintTable(FindTable("Student"))
parsed = parse_sql()
print(parsed)
# for item in parsed:
PrintTable(Select(FindTable("Student"), ["No"]))

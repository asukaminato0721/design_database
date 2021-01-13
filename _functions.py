# 一些用于处理sql语句的函数
import sqlparse
from DataStructureModule import *
from sql_parse.lexical_analysis import parse_sql
from dataclasses import dataclass

reserved_words = ["+", "-", ">", "<", "="]


@dataclass
class Parsed:
    type: str = ""
    # https://stackoverflow.com/questions/53632152/why-cant-dataclasses-have-mutable-defaults-in-their-class-attributes-declaratio
    columns: list = field(default_factory=list)
    setOrInsert_value: list = field(default_factory=list)
    tables: list = field(default_factory=list)
    wheres: list = field(default_factory=list)


def extract_tables(table_str: str) -> dict:
    """
    提取表的别名，输入sql语句，返回一个字典 key为表别名，value为表真名
    """
    tables = {}
    table_list = table_str.split(",")
    for item in table_list:
        if item.split(" ")[1] in tables:
            if item.split(" ")[0] != tables[item.split(" ")[1]]:
                return {}
        else:
            tables[item.split(" ")[1]] = item.split(" ")[0]
    return tables


def format_parsed(parsed: Parsed, tables: dict = {}, tables_cols: dict = {}):
    if len(parsed.tables) == 1:
        parsed.columns = [
            list(i.split("."))[1] if "." in i else i for i in parsed.columns
        ]
    else:
        for i in range(len(parsed.columns)):
            if "." in parsed.columns[i]:  # 属性名已经有前缀表名
                tableName = list(parsed.columns[i].split("."))[0]
                if tableName in tables.keys():
                    tableName = tables[tableName]
            else:
                columnName = parsed.columns[i]
                for key in tables_cols.keys():
                    if columnName in tables_cols[key]:
                        parsed.columns[i] = key + "." + columnName
    for i in range(len(parsed.tables)):
        parsed.tables[i] = list(parsed.tables[i].split(" "))[0]


def getResult(
    cmd: str = "select * from Student",
) -> Tuple[str, List[str], List[tuple]]:
    parsed_l = parse_sql(cmd)
    print(parsed_l)
    parsed = Parsed()
    tables = {}
    result_table = Table()
    for i in range(len(parsed_l)):
        if i == 0:
            parsed.type = parsed_l[0][0].lower()  # 获取sql语句的类型
        else:
            if parsed.type == "select":
                parsed.columns = list(parsed_l[0][1].split(","))
                if parsed_l[i][0].lower() == "from":
                    parsed.tables = list(parsed_l[i][1].split(","))
                    tables = extract_tables(parsed_l[i][1])
                elif parsed_l[i][0].lower() == "where":
                    parsed.wheres = list(parsed_l[i][1].split(","))
    format_parsed(parsed, tables, {})
    tables_cols = {
        table: [field.FieldName for field in FindTable(table).TableField]
        for table in parsed.tables
    }
    format_parsed(parsed, tables, tables_cols)  # 格式化
    result_table = Select(
        Where(
            From(FindTable(parsed.tables[0])),
            lambda line: line[parsed.columns[0]] > 0,
        ),
        parsed.columns,
    )
    return convertTableToJson(result_table)


if __name__ == "__main__":
    print(getResult("select * from Student"))

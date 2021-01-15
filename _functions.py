# 一些用于处理sql语句的函数
import sqlparse
from DataStructureModule import *
from sql_parse.lexical_analysis import parse_sql
from dataclasses import dataclass

calculators = ["+", "-", ">", "<", "="]


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
        if len(item.split(" "))==1:
            continue
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
                parsed.columns[i]=tableName+"."+parsed.columns[i].split(".")[1]
            else:
                columnName = parsed.columns[i]
                for key in tables_cols.keys():
                    if columnName in tables_cols[key]:
                        if "." in parsed.columns[i]:
                            print("error:column name is in multiple tables")
                        else:
                            parsed.columns[i] = key + "." + columnName
                    
    for i in range(len(parsed.tables)): #除去from中表的别名
        parsed.tables[i] = list(parsed.tables[i].split(" "))[0]
    where_str=parsed.wheres[0]
    pos=[]
    for ptr in range(len(where_str)):
        if where_str[ptr] in calculators:
            pos.append(ptr)
    temp_str=""
    print("pos",pos)
    for i in range(len(pos)):
        if i==0:
            temp_str=where_str[0:pos[i]]
        else:
            temp_str=where_str[pos[i-1]+1:pos[i]]
        if temp_str.isdigit():
            parsed.wheres.append(["number",temp_str])
        else:
            if temp_str!="":
                parsed.wheres.append(["identifier",temp_str])
        parsed.wheres.append(["calculator",where_str[pos[i]]])
        if i==len(pos)-1:
            temp_str=where_str[pos[i]+1:len(where_str)]
            if temp_str.isdigit():
                parsed.wheres.append(["number",temp_str])
            else:
                parsed.wheres.append(["identifier",temp_str])
    parsed.wheres.remove(parsed.wheres[0])
    for i in range(len(parsed.wheres)):
        if parsed.wheres[i][0]=="identifier":
            if len(parsed.tables)==1:
                parsed.wheres[i][1]=parsed.wheres[i][1].split(".")[-1]
                continue
            if "." in parsed.wheres[i][1]:
                if parsed.wheres[i][1].split(".")[0] in tables.keys(): #前缀表名为别名
                    parsed.wheres[i][1]=tables[parsed.wheres[i][1].split(".")[0]]+"."+parsed.wheres[i][1].split(".")[1]
                elif parsed.wheres[i][1].split(".")[0] in tables_cols.keys():#前缀表名为本名
                    pass
                else:
                    print("error:cannot identify table")
            else:
                columnName = parsed.wheres[i][1]
                for key in tables_cols.keys():
                    if columnName in tables_cols[key]:
                        if "." in parsed.wheres[i][1]:
                            print("error:column name is in multiple tables")
                        else:
                            parsed.wheres[i][1] = key + "." + columnName
def getResult(
    cmd: str = "select * from Student",
) -> Tuple[str, List[str], List[tuple]]:
    parsed_l = parse_sql(cmd)
    print(parsed_l)
    parsed = Parsed()
    tables_other_name = {}
    result_table = Table()
    for i in range(len(parsed_l)):
        if i == 0:
            parsed.type = parsed_l[0][0].lower()  # 获取sql语句的类型
        else:
            if parsed.type =="select":
                parsed.columns = list(parsed_l[0][1].split(","))
                if parsed_l[i][0].lower() == "from":
                    parsed.tables = list(parsed_l[i][1].split(","))
                    tables_other_name = extract_tables(parsed_l[i][1])
                elif parsed_l[i][0].lower() == "where":
                    parsed.wheres = list(parsed_l[i][1].split(","))
            elif parsed.type =="delete":
                if parsed_l[i][0].lower() == "from":
                    parsed.tables = list(parsed_l[i][1].split(","))
                    tables_other_name = extract_tables(parsed_l[i][1])
                elif parsed_l[i][0].lower() == "where":
                    parsed.wheres = list(parsed_l[i][1].split(","))
    # format_parsed(parsed, tables_other_name, {})
    for i in range(len(parsed.tables)): #除去from中表的别名
        parsed.tables[i] = list(parsed.tables[i].split(" "))[0]
    tables_cols = {
        table: [field.FieldName for field in FindTable(table).TableField]
        for table in parsed.tables
    }
    format_parsed(parsed, tables_other_name, tables_cols)  # 格式化
    print(parsed.type,parsed.columns,parsed.tables,parsed.wheres)
    where_eval_str=""
    for item in parsed.wheres:
        if item[0]=="identifier":
            where_eval_str+="line[\""+item[1]+"\"]"
        else:
            where_eval_str+=item[1]
    print(where_eval_str)
    if parsed.type=="select":#select 语句
        tables=[]
        for item in parsed.tables:
            tables.append(FindTable(item))
        result_table=Select(
            Where(
                From(
                    *tables
                ),
                lambda line:eval(where_eval_str)
            ),
            parsed.columns
        )
    if parsed.type=="delete":# delete 语句
        tables=[]
        for item in parsed.tables:
            tables.append(FindTable(item))
        result_table=Delete(
                From(
                    *tables
                ),
                lambda line:eval(where_eval_str)
        )
    return convertTableToJson(result_table)

#解析sql语句
def sql(sql):
    


    return

if __name__ == "__main__":
    PrintTable(FindTable("Score"))
    print(getResult("select a.No,b.Score from Student a,Score b where a.No>10013"))

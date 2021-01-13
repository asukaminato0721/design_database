# 本模块定义了数据结构，并实现查询、删除、增加、修改等功能
import time
import pickle
from enum import Enum
from dataclasses import dataclass, field
from typing import Callable, List, Tuple
import os
import pathlib

# ======================== Const definition ========================
FILE_PATH = os.path.join(
    pathlib.Path(__file__).parent.absolute(), "database.db"
)


# ======================== Const definition end ========================

# ======================== Class definition ========================


class DataType(Enum):
    # Binary Data (no size limits)
    BIN = 0
    # Interger Data (signed 64 bits)
    INTERGER = 1
    # String (no size limits)
    TEXT = 3


@dataclass
class Field:
    FieldName: str = ""
    FieldType: DataType = DataType.BIN
    FieldIsPK: bool = False
    FieldAllowNull: bool = True


@dataclass
class Table:
    # Name of the Table
    TableName: str = ""
    # cols in the table
    # Eg: [Field1(Name,TEXT),Field2(Gender,TEXT),Field3(Age,INTERGER)]
    TableField: List[Field] = field(default_factory=List[Field])
    # Rows in the table
    # Eg: [(Zhangsan,Male,14),(lisi,Male,16),(Wanghong,Female,15)]
    TableData: List[tuple] = field(default_factory=List[tuple])


# ======================== Class definition end ========================


# ======================== Global variable ========================


# ======================== Global variable end ========================


def ReadFile(filePath: str) -> list:
    """
    Open and Read Database File from `filePath` and return database object
    """
    try:
        DBFileHandle = open(filePath, "rb")
    except:
        print("Error: 没有找到文件或读取文件失败")
        return None
    else:
        Database = pickle.load(DBFileHandle)
        DBFileHandle.close()
        return Database


def SaveFile(filePath: str, database: list) -> int:
    """
    Save `database` from memory to file
    """
    try:
        DBFileHandle = open(filePath, "wb")
    except:
        print("Error: 没有找到文件或读取文件失败")
        return None
    pickle.dump(database, DBFileHandle)
    DBFileHandle.close()
    return 0


def Create(name: str, fieldlist: list, datalist: list) -> Table:
    if FindTable(name) != None:
        print("Error:表已存在")
        return None
    db = ReadFile(FILE_PATH)
    table_1 = Table(name, fieldlist, datalist)
    db.append(table_1)
    SaveFile(FILE_PATH, db)
    return table_1


def Drop(name: str):
    db = ReadFile(FILE_PATH)
    for tName in db:
        if tName.TableName == name:
            db.remove(tName)
    SaveFile(FILE_PATH, db)


def From(*tables: Table) -> Table:
    """
    Calculating Cartesian product of `tables1,[tables2,tables3,...]`
    """
    if len(tables) == 1:
        return tables[0]

    retTable = Table(TableName=tables[0].TableName)
    for fields in tables[0].TableField:
        retTable.TableField.append(
            Field(
                tables[0].TableName + "." + fields.FieldName,
                fields.FieldType,
                fields.FieldIsPK,
                fields.FieldAllowNull,
            )
        )
    retTable.TableData = tables[0].TableData
    for t in tables[1:]:
        retTable.TableName += "&" + t.TableName

        for fields in t.TableField:
            retTable.TableField.append(
                Field(
                    t.TableName + "." + fields.FieldName,
                    fields.FieldType,
                    fields.FieldIsPK,
                    fields.FieldAllowNull,
                )
            )

        rowsBuff = [
            col1 + col2 for col1 in retTable.TableData for col2 in t.TableData
        ]
        retTable.TableData = rowsBuff
    return retTable


def Where(table: Table, constraints: Callable[[dict], bool]) -> Table:
    # TODO : Need to refactor
    """
    Get rows from `table` where rows fit `constraints`\n
    `constraints` should be a lambda expression\n
    Eg :`Where(table_1,lambda line:line['Age'] <= 10 and line.['Gender']=='M')`
    """
    retTable = Table(TableField=table.TableField, TableName=table.TableName)
    fieldName = [field.FieldName for field in table.TableField]
    for rows in table.TableData:
        line = dict(zip(fieldName, rows))
        if constraints(line):
            retTable.TableData.append(tuple(line[fname] for fname in fieldName))
    return retTable


def Select(table: Table, fieldList: list) -> Table:
    """
    Get cols from `table` where fieldName in `fieldList`\n
    `fieldList` should be a list consists of string or '*'\n
    Eg :`Select(table_1,['Age','Gender'])`
    """
    if fieldList == "*":
        return table

    retTable = Table(TableName=table.TableName)

    selectFieldIndex = []
    for tfName in fieldList:
        for i, TableFieldi in enumerate(table.TableField):
            if TableFieldi.FieldName == tfName:
                retTable.TableField.append(TableFieldi)
                selectFieldIndex.append(i)
    for row in table.TableData:
        rowCache = [row[i] for i in selectFieldIndex]
        retTable.TableData.append(tuple(rowCache))
    return retTable


def Insert(table: Table, row: tuple) -> Table:
    # TODO : Datatype Check
    """
    Insert `row` into `table`
    """
    if len(table.TableField) == len(row):
        pkIndex = 0
        for f in table.TableField:
            if f.FieldIsPK:
                pkIndex = table.TableField.index(f)
        for data in table.TableData:
            if row[pkIndex] == data[pkIndex]:
                print("Error:插入重复数据")
                return None
        table.TableData.append(row)
        table.TableData.sort(key=lambda tup: tup[pkIndex])
        db = ReadFile(FILE_PATH)
        for tName in db:
            if tName.TableName == table.TableName:
                temp = db.index(tName)
                db.remove(tName)
                db.insert(temp, table)
                SaveFile(FILE_PATH, db)
        return table
    else:
        print("Error: 插入数据与属性个数不符")
        return None


def Delete(table: Table, constraints: Callable[[dict], bool]) -> Table:
    # TODO : Need to refactor
    """
    Delete rows from `table` where rows fit `constraints`\n
    `constraints` should be a lambda expression\n
    Eg :`Where(table_1,lambda line:line['Age'] <= 10 and line.['Gender']=='M')`
    """
    fieldName = [field.FieldName for field in table.TableField]
    rowsData = []
    for tableRow in table.TableData:
        line = {fName: tableRow[fieldName.index(fName)] for fName in fieldName}
        if not constraints(line):
            rowsData.append(tableRow)
    table.TableData = rowsData
    db = ReadFile(FILE_PATH)
    for tName in db:
        if tName.TableName == table.TableName:
            temp = db.index(tName)
            db.remove(tName)
            db.insert(temp, table)
            SaveFile(FILE_PATH, db)
    return table


def Update(
    table: Table,
    fieldlist: list,
    valuelist: list,
    constraints: Callable[[dict], bool],
) -> Table:
    """
    Update `table` set field1=value1,field2=value2,... where fields fit `constraints`\n
    `constraints` should be a lambda expresssion\n
    Eg :`Where(table_1,lambda line:line['Age'] <= 10 and line.['Gender']=='M')`
    """
    fieldName = []
    rowsData = []
    pkIndex = 0
    for field in table.TableField:
        fieldName.append(field.FieldName)
        if field.FieldIsPK:
            pkIndex = table.TableField.index(field)

    for tableRow in table.TableData:
        line = {fName: tableRow[fieldName.index(fName)] for fName in fieldName}
        if not constraints(line):
            rowsData.append(tableRow)
        else:
            temp = list(tableRow)
            for fName in fieldlist:
                temp[fieldName.index(fName)] = valuelist[fieldlist.index(fName)]
            rowsData.append(tuple(temp))
    table.TableData = rowsData
    table.TableData.sort(key=lambda tup: tup[pkIndex])
    db = ReadFile(FILE_PATH)
    for tName in db:
        if tName.TableName == table.TableName:
            temp = db.index(tName)
            db.remove(tName)
            db.insert(temp, table)
            SaveFile(FILE_PATH, db)
    return table


def PrintTable(table: Table) -> None:
    """
    Print `table`
    """
    if not table:
        return
    print(f"[Table : {table.TableName}]")
    for cols in table.TableField:
        print(f"{cols.FieldName:>14}", end="")
    print()
    for rows in table.TableData:
        for data in rows:
            print(f"{data:>14}", end="")
        print()
    print()


def FindTable(name: str) -> Table:
    db = ReadFile(FILE_PATH)
    table = Table()
    for tName in db:
        if tName.TableName == name:
            table = tName
            return table
    return None


def convertTableToJson(table: Table) -> Tuple[str, List[str], List[tuple]]:
    """
    返回 `表名` `列名` `每行的元组构成的列表`
    """
    return (
        table.TableName,
        [x.FieldName for x in table.TableField],
        table.TableData,
    )


def _Test():

    """
                        FieldName FieldType FieldIsPK FieldAllowNull
    CREATE TABLE Student(Name,    TEXT,     False,    True,
                        No,       INTERGER, True,     False,
                        Gender,   TEXT,     False,    True)

    """
    print(
        convertTableToJson(
            Table(
                "Score",
                [
                    Field("No", DataType.INTERGER, True, False),
                    Field("Score", DataType.INTERGER, False, True),
                ],
                [
                    (10011, 89),
                    (10012, 92),
                    (10013, 99),
                    (10014, 77),
                    (10015, 52),
                    (10016, 55),
                    (10017, 87),
                    (10018, 80),
                    (10019, 91),
                    (10020, 100),
                ],
            )
        )
    )
    PrintTable(FindTable("Student"))
    PrintTable(FindTable("Score"))
    # SELECT Name,Score,No FROM Student,Score WHERE Student.No=Score.No and Score.Score>=60
    PrintTable(
        Select(
            Where(
                From(FindTable("Student"), FindTable("Score")),
                lambda line: line["Student.No"] == line["Score.No"]
                and line["Score.Score"] >= 60,
            ),
            ["Student.No", "Student.Name", "Score.Score"],
        )
    )

    # DELETE FROM table_2 WHERE table_2.Score < 90
    PrintTable(Delete(FindTable("Score"), lambda line: line["Score"] < 90))

    # INSERT INTO table_2 VALUES (20012,59)
    PrintTable(Insert(FindTable("Score"), (20012, 59)))

    # UPDATE table_2 SET No='20019',SCORE='100' WHERE No='10019'
    PrintTable(
        Update(
            FindTable("Score"),
            ["No", "Score"],
            [20019, 100],
            lambda line: line["No"] == 10019,
        )
    )
    # SaveFile(FILE_PATH, db)
    pass


if __name__ == "__main__":
    _Test()

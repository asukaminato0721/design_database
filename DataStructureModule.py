# 本模块定义了数据结构，并实现查询、删除、增加、修改等功能
import time
import pickle
from enum import Enum
from dataclasses import dataclass, field
from typing import List

# ======================== Const definition ========================
FILE_PATH = 'database.db'


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
    FieldName: str = ''
    FieldType: DataType = DataType.BIN
    FieldIsPK: bool = False
    FieldAllowNull: bool = True


@dataclass
class Table:
    # Name of the Table
    TableName: str = ''
    # cols in the table
    # Eg: [Field1(Name,TEXT),Field2(Gender,TEXT),Field3(Age,INTERGER)]
    TableField: List[Field] = field(default_factory=list)
    # Rows in the table
    # Eg: [(Zhangsan,Male,14),(lisi,Male,16),(Wanghong,Female,15)]
    TableData: List[tuple] = field(default_factory=list)

# ======================== Class definition end ========================


# ======================== Global variable ========================


# ======================== Global variable end ========================


def ReadFile(filePath: str) -> list:
    """
    Open and Read Database File from `filePath` and return database object
    """
    try:
        DBFileHandle = open(filePath, 'rb')
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
        DBFileHandle = open(filePath, 'wb')
    except:
        print("Error: 没有找到文件或读取文件失败")
        return None
    pickle.dump(database, DBFileHandle)
    DBFileHandle.close()
    return 0


def From(*tables: Table) -> Table:
    """
    Calculating Cartesian product of `tables1,[tables2,tables3,...]`
    """
    if(len(tables) == 1):
        return tables[0]

    retTable = Table()
    retTable.TableName = tables[0].TableName
    for fields in tables[0].TableField:
        retTable.TableField.append(
            Field(tables[0].TableName+'.'+fields.FieldName,
                  fields.FieldType,
                  fields.FieldIsPK,
                  fields.FieldAllowNull
                  )
        )
    retTable.TableData = tables[0].TableData
    for t in tables[1:]:
        retTable.TableName += '&' + t.TableName

        for fields in t.TableField:
            retTable.TableField.append(
                Field(t.TableName+'.'+fields.FieldName,
                      fields.FieldType,
                      fields.FieldIsPK,
                      fields.FieldAllowNull
                      )
            )

        rowsBuff = []
        for col1 in retTable.TableData:
            for col2 in t.TableData:
                rowsBuff.append(col1+col2)
        retTable.TableData = rowsBuff
    return retTable


def Where(table: Table, constraints) -> Table:
    # TODO : Need to refactor
    """
    Get rows from `table` where rows fit `constraints`\n
    `constraints` should be a lambda expression\n
    Eg :`Where(table_1,lambda line:line['Age'] <= 10 and line.['Gender']=='M')`
    """
    retTable = Table()
    retTable.TableField = table.TableField
    retTable.TableName = table.TableName

    fieldName = []
    for field in table.TableField:
        fieldName.append(field.FieldName)
    for rows in table.TableData:
        line = dict()
        for i in range(len(fieldName)):
            line[fieldName[i]] = rows[i]
        if(constraints(line) == True):
            col = []
            for fname in fieldName:
                col.append(line[fname])
            retTable.TableData.append(tuple(col))
    return retTable


def Select(table: Table, fieldList: list) -> Table:
    """
    Get cols from `table` where fieldName in `fieldList`\n
    `fieldList` should be a list consists of string or '*'\n
    Eg :`Select(table_1,['Age','Gender'])`
    """
    if(fieldList == '*'):
        return table

    retTable = Table()
    retTable.TableName = table.TableName

    selectFieldIndex = []
    for tfName in fieldList:
        for i in range(len(table.TableField)):
            if(table.TableField[i].FieldName == tfName):
                retTable.TableField.append(table.TableField[i])
                selectFieldIndex.append(i)
    for row in table.TableData:
        rowCache = []
        for i in selectFieldIndex:
            rowCache.append(row[i])
        retTable.TableData.append(tuple(rowCache))
    return retTable


def Insert(table: Table, row: tuple) -> Table:
    # TODO : Datatype Check
    """
    Insert `row` into `table`
    """
    if(len(table.TableField) == len(row)):
        table.TableData.append(row)
        return table
    else:
        print("Error: 插入数据与属性个数不符")
        return None


def Delete(table: Table, constraints) -> Table:
    # TODO : Need to refactor
    """
    Delete rows from `table` where rows fit `constraints`\n
    `constraints` should be a lambda expression\n
    Eg :`Where(table_1,lambda line:line['Age'] <= 10 and line.['Gender']=='M')`
    """
    fieldName = []
    rowsData = []
    for field in table.TableField:
        fieldName.append(field.FieldName)

    for tableRow in table.TableData:
        line = dict()
        for fName in fieldName:
            line[fName] = tableRow[fieldName.index(fName)]
        if(constraints(line) == False):
            rowsData.append(tableRow)
    table.TableData = rowsData
    return table


def PrintTable(table: Table) -> None:
    """
    Print `table`
    """
    print('[Table : %s]' % table.TableName)
    for cols in table.TableField:
        print("%12.10s" % cols.FieldName, end='')
    print()
    for rows in table.TableData:
        for data in rows:
            print("%12.10s" % str(data), end='')
        print()
    print()
    return


def _Test():
    db = ReadFile(FILE_PATH)
    table_1 = Table('Student',
                    [
                        Field('Name', DataType.TEXT, False, True),
                        Field('No', DataType.INTERGER, True, False),
                        Field('Gender', DataType.TEXT, False, True)
                    ],
                    [
                        ('Mike', 10011, 'M'),
                        ('Louise', 10012, 'M'),
                        ('Monika', 10013, 'F'),
                        ('Jason', 10014, 'M'),
                        ('Alice', 10015, 'F'),
                        ('Ulrica', 10016, 'F'),
                        ('Bill', 10017, 'F'),
                        ('Alex', 10018, 'M'),
                        ('Jeb', 10019, 'F'),
                        ('Max', 10020, 'M'),
                    ]
                    )
    table_2 = Table('Score',
                    [
                        Field('No', DataType.INTERGER, True, False),
                        Field('Score', DataType.INTERGER, False, True)
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
                    ]
                    )

    db = [table_1, table_2]

    # PrintTable(Where(table_2, lambda line: line['Score'] > 90))
    # PrintTable(Select(table_1, ['No', 'Name']))

    # SELECT * FROM table_1, table_2 WHERE Student.No=Score.No AND Score.Score>=60

    PrintTable(Where(From(table_1, table_2),
                     lambda line: line['Student.No'] == line['Score.No'] and line['Score.Score'] >= 60))

    # DELETE FROM table_2 WHERE table_2.Score < 90
    PrintTable(Delete(table_2, lambda line: line['Score'] < 90))
    # INSERT INTO table_2 values (20012,59)
    PrintTable(Insert(table_2, (20012, 59)))

    SaveFile(FILE_PATH, db)
    pass


if __name__ == "__main__":
    _Test()

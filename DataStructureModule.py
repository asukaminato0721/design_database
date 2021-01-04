# 本模块定义了数据结构，并实现查询、删除、增加、修改等功能
import pickle
from enum import Enum
from dataclasses import dataclass, field
from typing import List

# ======================== Const definition ========================
# MAX Table Name length (bytes)
MAX_TABLE_NAME_LEN = 64
# MAX Field length (bytes)
MAX_FIELD_NAME_LEN = 32
MAX_FIELD_TYPE_LEN = 4
MAX_FIELD_TAGS_LEN = 4

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
# Database file handle
DBFileHandle: object = None

# ======================== Global variable end ========================


def OpenFile(filePath: str) -> list:
    """
    Open and Read Database File from `filePath` and return database object
    """
    global DBFileHandle
    try:
        DBFileHandle = open(filePath, 'rb+')
    except:
        print("Error: 没有找到文件或读取文件失败")
        return None
    else:
        Database = pickle.load(DBFileHandle)
        return Database


def SaveFile(database: list) -> int:
    """
    Save `database` from memory to file 
    """
    global DBFileHandle
    if(DBFileHandle == None):
        print("Error: 尚未打开文件")
        return 1
    pickle.dump(database, DBFileHandle)
    return 0


def CloseFile():
    """
    Close the Database file
    """
    global DBFileHandle
    DBFileHandle.close()


def CrossJoin(*tables: Table) -> Table:
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


def Where(table: Table, constraints: tuple) -> Table:
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
    """
    Insert `row` into `table`
    TODO : Datatype Check
    """
    if(len(table.TableField) == len(row)):
        table.TableData.append(row)
        return table
    else:
        print("Error: 插入数据与属性个数不符")
        return None


def Delete(table: Table, constraints: tuple) -> Table:
    # TODO : Need to refactor
    """
    Delete rows from `table` where rows fit `constraints`\n
    `constraints` should be a lambda expression\n
    Eg :`Where(table_1,lambda line:line['Age'] <= 10 and line.['Gender']=='M')`
    """
    fieldName = []
    delIndex = []
    for field in table.TableField:
        fieldName.append(field.FieldName)
    for i in range(len(table.TableData)):
        line = dict()
        for j in range(len(fieldName)):
            line[fieldName[j]] = table.TableData[i][j]
        if(constraints(line) == True):
            delIndex.append(i)
    for i in delIndex:
        table.TableData.pop(i)
    return table


def PrintTable(table: Table) -> None:
    """
    Print `table`
    """
    print('Table Name : '+table.TableName)
    for cols in table.TableField:
        print(cols.FieldName, end='\t')
    print()
    for rows in table.TableData:
        for data in rows:
            print(data, end='\t')
        print()
    print()
    return


def _Test():
    db = OpenFile('database.db')
    table_1 = Table('Student',
                    [
                        Field('Name', DataType.TEXT, False, True),
                        Field('No', DataType.INTERGER, True, False),
                        Field('Gender', DataType.TEXT, False, True)
                    ],
                    [
                        ('Mike', 10011, 'M'),
                        ('Louise', 10012, 'M'),
                        ('Monika', 10013, 'F')
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
                        (10013, 99)
                    ]
                    )

    PrintTable(db[0])

    db = [table_1, table_2]

    PrintTable(Where(table_2, lambda line: line['Score'] > 90))
    PrintTable(Select(table_1, ['No', 'Name']))
    PrintTable(Delete(table_2, lambda line: line['Score'] < 90))
    PrintTable(table_2)

    SaveFile(db)
    CloseFile()


if __name__ == "__main__":
    _Test()

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

Database: list = []


# ======================== Global variable end ========================

def OpenFile(filePath: str) -> int:
    """
    Open and Read Database File
    """
    global DBFileHandle, Database
    try:
        DBFileHandle = open(filePath, 'rb+')
    except:
        print("Error: 没有找到文件或读取文件失败")
        return 1
    else:
        Database = pickle.load(DBFileHandle)
        return 0


def SaveFile() -> int:
    """
    Save DataBase File
    """
    if(DBFileHandle == None):
        print("Error: 尚未打开文件")
        return 1
    pickle.dump(Database, DBFileHandle)
    return 0


def CrossJoin(*tables: Table) -> Table:
    """
    Calculating Cartesian product
    """
    retTable = tables[0]
    for t in tables[1:]:
        retTable.TableName += t.TableName
        retTable.TableField += t.TableField
        rowsBuff = []
        for col1 in retTable.TableData:
            for col2 in t.TableData:
                rowsBuff.append(col1+col2)
        retTable.TableData = rowsBuff
    return retTable


def PrintTable(table: Table) -> None:
    """
    Print Table
    """
    for c in table.TableField:
        print(c.FieldName, end='\t')
    print()
    for r in table.TableData:
        for rc in r:
            print(rc, end='\t')
        print()
    print()
    return


def _Test():
    OpenFile('database.db')
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
    PrintTable(table_1)
    PrintTable(table_2)
    PrintTable(CrossJoin(table_1, table_2))


if __name__ == "__main__":
    _Test()

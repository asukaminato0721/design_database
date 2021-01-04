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

# Open and Read Database File
def OpenFile(filePath: str) -> int:
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
    if(DBFileHandle == None):
        print("Error: 尚未打开文件")
        return 1
    pickle.dump(Database, DBFileHandle)
    return 0


if __name__ == "__main__":
    OpenFile('database.db')
    print(Database)

    Database = [Table('Student',
                      [
                          Field('Name', DataType.TEXT, False, True),
                          Field('No', DataType.INTERGER, True, False),
                          Field('Gender', DataType.TEXT, False, True)
                      ],
                      [
                          ('ZhangSan', 10011, 'M'),
                          ('LiSi', 10012, 'M'),
                          ('WangWu', 10013, 'F')
                      ]
                      ),
                Table('Score',
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
                ]
    SaveFile()

# 本模块定义了数据结构，并实现查询、删除、增加、修改等功能
import pickle
from enum import Enum
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


class Field:
    FieldName: str = ''
    FieldType: DataType = DataType.BIN
    FieldIsPK: bool = False
    FieldAllowNull: bool = True


class Table:
    # Name of the Table
    TableName: str = ''
    # cols in the table
    # Eg: [Field1(Name,TEXT),Field2(Gender,TEXT),Field3(Age,INTERGER)]
    TableField: list = []
    # Rows in the table
    # Eg: [(Zhangsan,Male,14),(lisi,Male,16),(Wanghong,Female,15)]
    TableData: list = []

# ======================== Class definition end ========================


# ======================== Global variable ========================
# Database file handle
DBFileHandle: object = None

Database: list = []


# ======================== Global variable end ========================

# Open and Read Database File
def OpenFile(filePath: str) -> int:
    global DBFileHandle
    try:
        DBFileHandle = open(filePath, 'r+')
    except:
        print("Error: 没有找到文件或读取文件失败")
        return 1
    else:
        pickle.load(DBFileHandle, Database)
        return 0


def SaveFile() -> int:
    if(DBFileHandle == None):
        return 1
    pickle.dump(Database, DBFileHandle)
    return 0


if __name__ == "__main__":
    OpenFile('database.db')

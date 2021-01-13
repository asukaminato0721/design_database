import sqlite3, os, pathlib


def getResult(cmd: str):
    DB = os.path.join(
        pathlib.Path(__file__).parent.parent.absolute(), "db.sqlite3"
    )
    c = sqlite3.connect(DB)
    conn = c.cursor()
    res = conn.execute(cmd).fetchall()
    c.close()
    return res


def getHeadings(tableName: str):
    DB = os.path.join(
        pathlib.Path(__file__).parent.parent.absolute(), "db.sqlite3"
    )
    c = sqlite3.connect(DB)
    conn = c.cursor()
    res = conn.execute(f"select * from {tableName} where 1=0;").fetchall()
    c.close()
    return [d[0] for d in conn.description]


if __name__ == "__main__":
    print(getResult("select * from Student"))
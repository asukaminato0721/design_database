from typing import List
from DataStructureModule import *


def 预处理(cmd: str) -> str:
    """
    >>> 预处理("select age, name from Student")
    'select age,name from Student'
    """
    while ", " or " ," or "= " or " =" in cmd:
        cmd = (
            cmd.replace(", ", ",")
            .replace(" ,", ",")
            .replace("= ", "=")
            .replace(" =", "=")
        )
    return cmd


def 切分(cmd: str) -> List[str]:
    """
    >>> 切分("select age,name from Student")
    ['select', 'age,name', 'from', 'Student']
    """
    return cmd.split()


def select_from(cols: List[str], *table: Table):
    return Select(From(*table), cols)


def select_from_where(
    cols: List[str], *table: Table, isok: Callable[[dict], bool]
):
    return Where(From(*table), isok)


def link(cmd: List[str]):
    """
    `select xxx from yyy`: select from\n
    `select xxx from yyy where`: select from where
    """
    return {
        ("select", "from"): select_from,
        ("select", "from", "where"): select_from_where,
    }[tuple(cmd[::2])](*map(x.split(",") for x in cmd[1::2]))


if __name__ == "__main__":
    from doctest import testmod

    testmod()

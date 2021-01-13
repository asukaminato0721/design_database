import sys
from django.shortcuts import render
from django.http import HttpResponse, HttpRequest
from database_data import all_tables, all_database


sys.path.append("..")
from _functions import getResult

# Create your views here.


def show(request: HttpRequest, db: str = "") -> HttpResponse:
    # https://www.runoob.com/django/django-form.html
    query_cmd = request.GET.get(
        "query_cmd", "select a.No from Student a where a.no = 1"
    )
    name, headings, values = getResult(query_cmd)
    return render(
        request,
        "index.html",
        context={
            **locals(),
            "all_tables": all_tables,
            "all_database": all_database,
        },
    )


def show_new(request: HttpRequest, db: str = ""):
    # https://www.runoob.com/django/django-form.html
    query_cmd = request.GET.get("query_cmd", "")
    is_valid = False
    提示语句 = ""
    if not query_cmd:
        提示语句 = f"您未输入查询"
    else:
        提示语句 = f"您输入的查询是{query_cmd}"
        is_valid = True
    query_result = [
        {"idx": 1, "item": 123},
        {"idx": 2, "item": 234},
    ]
    query_result = getResult(query_cmd)
    headings = query_result[0].keys()
    values = [x.values() for x in query_result]
    当前页面 = "select"
    return render(
        request,
        "index.html",
        context={
            **locals(),
            "all_tables": all_tables,
            "all_database": all_database,
        },
    )


def show_new(request: HttpRequest, db: str = ""):
    当前页面 = "show_new"
    names = all_tables.keys()
    headings = [x[0].keys() for x in all_tables.values()]
    values = [[x.values() for x in table] for table in all_tables.values()]
    return render(
        request,
        "show.html",
        context={
            "tables": zip(names, headings, values),
            "all_database": all_database,
            "all_tables": all_tables,
            **locals(),
        },
    )

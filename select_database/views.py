import sys
from django.shortcuts import render
from django.http import HttpResponse, HttpRequest
from django.urls.conf import path
from database_data import all_tables, all_database

# https://stackoverflow.com/a/45874916/13040423
sys.path.append("..")
from _functions import getResult


# Create your views here.


# Create your views here.
def select(request: HttpRequest, db: str = "") -> HttpResponse:
    # https://www.runoob.com/django/django-form.html
    query_cmd = request.GET.get(
        "query_cmd", "select a.No from Student a where a.no = 1"
    )
    print(query_cmd)
    提示语句 = ""
    if not query_cmd:
        提示语句 = f"您未输入查询"
    else:
        提示语句 = f"您输入的查询是{query_cmd}"
    query_result = [
        {"idx": 1, "item": 123},
        {"idx": 2, "item": 234},
    ]
    name, headings, values = getResult(query_cmd)
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


def select_new(request: HttpRequest, db: str = ""):
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

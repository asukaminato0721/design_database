from django.shortcuts import render
from django.http import HttpResponse, HttpRequest
from database_data import all_tables, all_database

# Create your views here.


# Create your views here.
def select(request: HttpRequest) -> HttpResponse:
    # https://www.runoob.com/django/django-form.html
    query_cmd = request.GET.get("query_cmd", "")
    当前数据库 = request.GET.get("db","暂无")
    is_valid = False
    if not query_cmd:
        query_cmd = f"您未输入查询"
    else:
        query_cmd = f"您输入的查询是{query_cmd}"
        is_valid = True
    query_result = [
        {"idx": 1, "item": 123},
        {"idx": 2, "item": 234},
    ]
    # query_result = sqlparser("语句")
    headings = query_result[0].keys()
    values = [x.values() for x in query_result]
    当前页面 = "select"
    return render(
        request,
        "select.html",
        context={
            **locals(),
            "all_tables": all_tables,
            "all_database": all_database,
        },
    )

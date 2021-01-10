from django.shortcuts import render, redirect
from django.http import HttpResponse, HttpRequest
from database_data import all_tables, all_database

# Create your views here.
def modify(request: HttpRequest):
    lines = 3
    当前页面 = "modify"
    当前数据库 = request.GET.get("db","暂无")
    return render(
        request,
        "modify.html",
        context={
            **locals(),
            "all_tables": all_tables,
            "all_database": all_database,
        },
    )

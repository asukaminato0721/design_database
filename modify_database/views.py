from django.shortcuts import render, redirect
from django.http import HttpResponse, HttpRequest
from database_data import all_tables, all_database

# Create your views here.
def modify(request: HttpRequest, db: str = ""):
    lines = 3
    当前页面 = "modify"
    query_cmd = request.GET.get("query_cmd", "")
    return render(
        request,
        "modify.html",
        context={
            **locals(),
            "all_tables": all_tables,
            "all_database": all_database,
        },
    )


def modify_new(request: HttpRequest, db: str = ""):
    return render(request, "index.html")

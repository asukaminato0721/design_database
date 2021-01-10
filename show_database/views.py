from django.shortcuts import render
from django.http import HttpResponse, HttpRequest
from database_data import all_tables, all_database

# Create your views here.


def show(request: HttpRequest, db: str = "") -> HttpResponse:
    当前页面 = "show"
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

from django.shortcuts import render
from django.http import HttpResponse, HttpRequest

# Create your views here.


def show(request: HttpRequest) -> HttpResponse:
    all_tables = {
        "test": [
            {"id": 1, "item": 123, "notnull": None},
            {"id": 2, "item": 237, "notnull": "str"},
        ],
        "test1": [
            {"idx": 1, "item": 123},
            {"idx": 2, "item": 234},
        ],
    }
    names = all_tables.keys()
    headings = [x[0].keys() for x in all_tables.values()]
    values = [[x.values() for x in table] for table in all_tables.values()]
    return render(
        request,
        "show.html",
        context={
            "tables": zip(names, headings, values),
        },
    )

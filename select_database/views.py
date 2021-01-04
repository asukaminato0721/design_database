from django.shortcuts import render
from django.http import HttpResponse, HttpRequest

# Create your views here.


# Create your views here.
def select(request: HttpRequest) -> HttpResponse:
    # https://www.runoob.com/django/django-form.html
    query_cmd = request.GET.get("query_cmd", "")
    if not query_cmd:
        query_cmd = f"您未输入查询"
    else:
        query_cmd = f"您输入的查询是{query_cmd}"
    return render(request, "select.html", context={"query_cmd": query_cmd})

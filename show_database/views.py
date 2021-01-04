from django.shortcuts import render
from django.http import HttpResponse, HttpRequest

# Create your views here.


# Create your views here.
def show(request: HttpRequest) -> HttpResponse:
    return render(request, "show.html")

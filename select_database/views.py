from django.shortcuts import render
from django.http import HttpResponse, HttpRequest

# Create your views here.


# Create your views here.
def select(request: HttpRequest) -> HttpResponse:
    return HttpResponse("This is a select page")

from django.shortcuts import render
from django.http import HttpResponse, HttpRequest

# Create your views here.
def modify(request: HttpRequest) -> HttpResponse:
    return HttpResponse("This is a modified page")

from django.shortcuts import render, redirect
from django.http import HttpResponse, HttpRequest

# Create your views here.
def modify(request: HttpRequest):
    lines = 3
    return render(request, "modify.html", context=locals())

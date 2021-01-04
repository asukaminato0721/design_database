from django.shortcuts import render, redirect
from django.http import HttpResponse, HttpRequest

# Create your views here.
def modify(request: HttpRequest):
    return render(request, "modify.html")

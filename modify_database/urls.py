from django.urls import path

from . import views

urlpatterns = [
    path("", views.modify, name="modify"),
    path("<slug:db>/", views.modify, name="modify"),
]
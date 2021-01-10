from django.urls import path

from . import views

urlpatterns = [
    path("", views.show, name="show"),
    path("<slug:db>/", views.show, name="show"),
]
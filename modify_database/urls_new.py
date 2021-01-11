from django.urls import path

from . import views

urlpatterns = [
    path("", views.modify_new),
    path("<slug:db>/", views.modify_new),
]
from django.urls import path

from . import views

urlpatterns = [
    path("", views.show_new),
    path("<slug:db>/", views.show_new),
    # path("<slug:db>/", views.show, name="show"),
]
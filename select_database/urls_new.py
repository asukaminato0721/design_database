from django.urls import path

from . import views

urlpatterns = [
    path("", views.select_new),
    # https://docs.djangoproject.com/zh-hans/3.1/intro/tutorial03/#writing-your-first-django-app-part-3
    path("<slug:db>/", views.select_new),
]
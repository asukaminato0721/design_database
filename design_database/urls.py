"""design_database URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/3.1/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
from django.contrib import admin
from django.urls import include, path
# 静态文件服务
from django.conf.urls.static import static

urlpatterns = [
    path("modify/", include("modify_database.urls")),
    path("modify_new/", include("modify_database.urls_new")),
    path("select/", include("select_database.urls")),
    path("select_new/", include("select_database.urls_new")),
    path("show/", include("show_database.urls")),
    path("show_new/", include("show_database.urls_new")),
    path("admin/", admin.site.urls),
    path("", include("show_database.urls")),
]+ static("/", document_root="./templates")
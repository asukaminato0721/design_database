#include<bits/stdc++.h>

using namespace std;
static int f = 0;
//初始界面显示；
int Start()
{
      if(f==0){
         cout<<" *------------------------------------------------*"<<endl;
         cout<<" |                                                | "<<endl;
         cout<<" |                  DBMS主界面                    | "<<endl;
         cout<<" |                                                | "<<endl;
         cout<<" *------------------------------------------------*" <<endl;
         cout<<"[用户登录请按1,用户注册请按2,退出系统请按"<<"'exit']"<<endl;
      }
     else
       ;
}

class TABLE
{
//表的数据结构
public:
    string table_name;
    vector<string> col_name;
    vector<string> type_value;
    int col_len;
    int line_len;
    string file_name;
    TABLE() {   }
};

class VIEW
{
 //视图的数据结构
public:
    vector<string> codes;
    string view_name;

    VIEW() { }
};

class INDEX
{
 //索引的数据结构
public:
    vector<string> codes;
    string index_name;

    INDEX() { }
};

class USER
{
    //用户数据结构
public:
	string name;
    string password;
	string right_super;
	string right_ordinary;

	string right_select;
	string right_insert;
	string right_delete;
	string right_update;

   //定义用户姓名密码读写权限
	USER(string name, string password, string right_super, string right_ordinary)
	{
		this->name = name;
		this->password = password;
		this->right_super = right_super;
		this->right_ordinary = right_ordinary;

	}

	USER()
	{
	    this->name = "\0";
	    this->password = "\0";
	    this->right_super = "false";
	    this->right_ordinary = "false";

    }

};

class TOKEN
{
 //索引的数据结构
public:
    vector<string> codes;
    string taken_name;

    TOKEN() { }
};

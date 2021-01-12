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
 //标志的数据结构
public:
    vector<string> codes;
    string taken_name;

    TOKEN() { }
};

//将读取到的字符串（文件中的一行）解析成User的各属性
USER StringToUser(string s)
{
	USER temp;
	string t = "";
	int i = 0;
	int len = s.length();

	while(s[i] == ' ')
            ++i;
	for( i; i < len; ++i){
        if(s[i] == ' ')
		   break;
		else t=t+s[i];

	}
	temp.name = t;
	t ="";
	while(s[i] == ' ')
        ++i;
	for( i; i < len; ++i){
        if(s[i] == ' ')
            break;
		else
		    t =t+ s[i];
	}

	temp.password = t;
	t ="";

	while(s[i] == ' ')
        ++i;
	for( i; i < len; ++i){
        if(s[i] == ' ')
            break;
		else
		    t =t+ s[i];
	}

	temp.right_super = t;

	t = "";

	while(s[i] == ' ')
        ++i;
	for( i; i < len; ++i){
        if(s[i] == ' ')
            break;
		else
		    t =t+ s[i];
	}

	temp.right_ordinary = t;

	return temp;
}

// 读取文件并比较，相同则返回该用户，否则返回空用户
USER UserCompare(string name, string password)
{
    USER temp;
    ifstream fin;
    fin.open("user.dat");//打开文件
    bool find = false;
    string s;
    while(!fin.eof())
    {
        getline(fin, s);
    	temp = StringToUser(s);
    	if(temp.name == name && temp.password == password)
        {
            fin.close();
            return temp;
        }
    }
    fin.close();
    USER t;
    return t;
}


//创建用户
void CreateUser()
{
    USER u;
    string name,password;
    printf("请输入注册用户名:");
    cin >> name;
    printf("\n");
    printf("请输入注册密码:");
    cin >> password;
    printf("\n");
    cout<<"-----------------------"<<endl;
    string privileges;
    cout << "请输入用户的类型：（超级用户:s  普通用户:o  不区分大小写）" << endl;
    cin >> privileges;
    printf("\n");
    u.name = name;
    u.password = password;
    //赋予读写权限
    for(int i = 0; i < privileges.length(); ++i){
         if(privileges[i]=='s'||privileges[i]=='S')
            u.right_super = "true";
         else if(privileges[i]=='o'||privileges[i]=='O')
            u.right_ordinary = "true";
    }

   //文件输入输出流，不存在则自动创建
    ofstream fout;
    fout.open("user.dat", ios::app);
    fout << u.name << " " << u.password << " " << u.right_super << " " << u.right_ordinary << "\n" << endl;
    fout.close();
}

void CreateGrant_op(string op, int &flag)//建立授权
{
    string s = "grant ";
    int f = 1;
    for(int i = 0; i < s.length(); ++i)
    {
        if(op[i] != s[i])
        {
            f = 0; break;
        }
    }
    if(f)
    {
        flag = 8;
        CreatGrant(op);
    }

    return;
}

void CreateRevoke_op(string op, int &flag)//建立撤销授权
{
    string s = "revoke ";
    int f = 1;
    for(int i = 0; i < s.length(); ++i)
    {
        if(op[i] != s[i])
        {
            f = 0; break;
        }
    }
    if(f)
    {
        flag = 8;
        CreatRevoke(op);
    }

    return;
}

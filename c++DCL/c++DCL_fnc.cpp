#include<bits/stdc++.h>
//0-98,

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

vector<TABLE> table;

vector<VIEW> view;

vector<INDEX> index;

vector<TOKEN> token;

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

//创建表
void CreateTable(string op)
{

    TABLE t;

    string temp = "create table ";
    int k = temp.length();

    t.table_name = "";
    for( ; (op[k]!='\n' || op[k]!='(' || op[k]!=' ') && k < op.length(); ++k)
        t.table_name += op[k];

    string s;
    while(getline(cin, s) && s != ")")
    {
        int f = 1;
        int i = 0;
        temp = "";
        while(!((s[i]>='a'&&s[i]<='z') || (s[i]>='A'&&s[i]<='Z')))
            ++i;
        for( ; s[i] != ' ' && i < s.length(); ++i)
        {
            temp += s[i];
        }
        t.col_name.push_back(temp);
        if(temp.length() == 0)
        {
            cout << "create语句格式错误……" << endl;
            return;
        }

        temp = "";
        while(!((s[i]>='a'&&s[i]<='z') || (s[i]>='A'&&s[i]<='Z')))
            ++i;
        for( ;s[i]!=' ' && s[i]!=',' && i < s.length(); ++i)
        {
            temp += s[i];
        }
        t.type_value.push_back(temp);
        if(temp.length() == 0)
        {
            cout << "create语句格式错误" << endl;
            return;
        }

    }

    table.push_back(t);

    ofstream fout;
    fout.open("table.dat", ios::app);
    cout<<"创建数据表"<<t.table_name<<",其属性名及属性类型如下所示："<<endl;

    for(int i = 0; i < t.col_name.size(); ++i)
        cout << t.col_name[i] <<"\t:" << t.type_value[i] << endl;
    fout << t.table_name << endl;
    fout.close();

    ifstream fin;
    fin.open("table.dat");


    char file[30];
    string tt = t.table_name + ".dat";
    for(int i = 0; i < tt.length(); ++i)
        file[i] = tt[i];
    file[tt.length()]='\0';
    ofstream outtxt;
    outtxt.open(file, ios::app);
    for(int i = 0; i < t.col_name.size(); ++i)
        outtxt<< t.col_name[i] << " "<<t.type_value[i]<<endl;

    fout.close();
    outtxt.close();

    cout << "创建数据表"+t.table_name <<"成功~"<< endl;

}

//创建视图函数
void CreatView(string s)
{
    string name;
    name = "create view ";
    int i = name.length();
    name = "";
    for( ; i < s.length() && s[i]!=' '; ++i)
        name += s[i];
    VIEW v;
    v.view_name = name;
    string t;
    while(getline(cin, t) && t != ")")
        v.codes.push_back(t);
    v.codes.push_back(")");
    view.push_back(v);

    cout<<"创建视图"<<v.view_name<<",其属性及条件如下："<<endl;
   // ofstream fout;
    //fout.open("view.dat", ios::app);
    cout <<"视图名\t：" <<v.view_name << endl;
    cout <<"属性\t：";
    //for(int i = 1; i < v.codes.size()-2; ++i)
    for(int r = 5;r<v.codes[1].length();r++)
       cout << v.codes[1][r];
    cout<<endl;
    cout <<"条件\t：";

    for(int rr = 6;rr<v.codes[2].length();rr++)
       cout << v.codes[2][rr];
    cout<<endl;

   ofstream outdata;
   outdata.open("view.dat",ios::app);//ios::app是尾部追加的意思
   outdata<<v.view_name<<endl;
   ofstream outtxt;
   string vname = v.view_name+".txt";
   char file[30];
   for(int i = 0; i < vname.length(); ++i)
        file[i] = vname[i];
    file[vname.length()] = '\0';
   outtxt.open(file,ios::app);
   for(int r = 8;r<v.codes[0].length();r++)
      outtxt<< v.codes[0][r];
   outtxt<<endl;
   outdata.close();
   outtxt.close();

    cout << "建立视图成功" << endl;
}

void CreatIndex(string s)
{
    string name;
    name = "create index ";
    int i = name.length();
    name = "";
    for( ; i < s.length() && s[i]!=' '; ++i)
        name += s[i];
    INDEX idx;
    idx.index_name = name;
    string t;
    while(getline(cin, t) && t != "")
        idx.codes.push_back(t);
    //idx.codes.push_back(";");
    index.push_back(idx);
    cout<<"创建索引"<<idx.index_name<<",其信息如下："<<endl;
    ofstream fout;
    fout.open("index.dat", ios::app);
    fout<<idx.index_name<<endl;
    cout <<"索引名\t：" <<idx.index_name << endl;
    cout <<"索引属性所在表\t：";
    //for(int i = 1; i < v.codes.size()-2; ++i)
    for(int r = 9;r<idx.codes[0].length();r++)
       cout <<idx.codes[0][r];
    cout<<endl;
    cout <<"索引属性\t：";

    for(int rr = 0;rr<idx.codes[1].length();rr++)
       cout << idx.codes[1][rr];
    cout<<endl;
   //  cout <<"索引属性\t：";
    cout <<"索引类型\t：";
    for(int rrr = 0;rrr<idx.codes[2].length();rrr++)
       cout << idx.codes[2][rrr];
    cout<<endl;

    char file[30];
    string tt = idx.index_name + ".txt";
    for(int i = 0; i < tt.length(); ++i)
        file[i] = tt[i];
    file[tt.length()]='\0';
    ofstream outtxt;
    outtxt.open(file,ios::app);
    for(int rr = 0;rr<idx.codes[1].length();rr++)
       outtxt<< idx.codes[1][rr];
    outtxt<<"|";
    for(int rrr = 0;rrr<idx.codes[2].length();rrr++)
       outtxt<< idx.codes[2][rrr];
    outtxt<<endl;
    fout.close();

    cout << "建立索引成功" << endl;
}

void CreatGrant(string s)
{
    string name;
    name = "grant ";
    int i = name.length();
    name = "";
    for( ; i < s.length() && s[i]!=' '; ++i)
        name += s[i];
    TOKEN tok;
    tok.taken_name=name;

    string t;
    while(getline(cin, t) && t != "")
        tok.codes.push_back(t);
    //idx.codes.push_back(";");
    token.push_back(tok);
    cout<<"对权限"<<tok.taken_name<<"的授权信息如下："<<endl;
    ofstream fout;
    fout.open("token.txt", ios::app);
    fout<<tok.taken_name<<"[";
    for(int r = 3;r<tok.codes[0].length();r++)
       fout<<tok.codes[0][r];
    fout<<"] To ";
    for(int rr = 3;rr<tok.codes[1].length();rr++)
       fout<< tok.codes[1][rr];
    fout<<endl;

    cout <<"权限名称   ：\t" <<tok.taken_name << endl;
    cout <<"所授予权限针对对象\t：";
    //for(int i = 1; i < v.codes.size()-2; ++i)
    for(int r = 3;r<tok.codes[0].length();r++)
       cout <<tok.codes[0][r];
    cout<<endl;
    cout <<"所授予给的用户\t ：";

    for(int rr = 3;rr<tok.codes[1].length();rr++)
       cout << tok.codes[1][rr];
    cout<<endl;
    fout.close();

    cout << "授权成功！" << endl;
}

void CreatRevoke(string s)
{
    string name;
    name = "revoke ";
    int i = name.length();
    name = "";
    for( ; i < s.length() && s[i]!=' '; ++i)
        name += s[i];
    TOKEN tok;
    tok.taken_name=name;

    string t;
    while(getline(cin, t) && t != "")
        tok.codes.push_back(t);
    //idx.codes.push_back(";");
    token.push_back(tok);
    cout<<"对权限"<<tok.taken_name<<"的回收信息如下："<<endl;
    ofstream fout;
    fout.open("token.txt", ios::app);
    fout<<tok.taken_name<<"[";
    for(int r = 3;r<tok.codes[0].length();r++)
       fout<<tok.codes[0][r];
    fout<<"] From ";
    for(int rr = 5;rr<tok.codes[1].length();rr++)
       fout<< tok.codes[1][rr];
    fout<<endl;

    cout <<"权限名称              \t ：" <<tok.taken_name << endl;
    cout <<"所授予权限针对对象 \t：";
    //for(int i = 1; i < v.codes.size()-2; ++i)
    for(int r = 3;r<tok.codes[0].length();r++)
       cout <<tok.codes[0][r];
    cout<<endl;
    cout <<"来源用户及回收操作类型\t ：";

    for(int rr = 5;rr<tok.codes[1].length();rr++)
       cout << tok.codes[1][rr];
    cout<<endl;

    fout.close();

    cout << "回收成功！" << endl;
}

USER uu;
void Help_op(string op, int& flag)//对应为help语句
{

	if(op == "help database")
	{

	    int have = 0;
		flag = 1;
		cout<<"数据库中数据表对象及其属性、属性类型如下："<<endl;
		for(int i = 0; i < table.size(); ++i)
		{
		    have = 1;
		    cout <<table[i].table_name<<"：" << endl;
		    for(int j = 0; j < table[i].col_name.size(); ++j)
                cout << "属性"<<":\t"<<table[i].col_name[j] << " \t"<<table[i].type_value[j]<<"\t"<<endl;

		    cout << endl;
		}
        cout<<"数据库中视图对象及其属性、属性类型如下："<<endl;
		for(int i = 0; i < view.size(); ++i)
		{
		    have = 1;
		    cout << view[i].view_name << ":" << endl;

		    cout <<"属性\t：";
           //for(int i = 1; i < v.codes.size()-2; ++i)
            for(int r = 5;r<view[i].codes[1].length();r++)
            cout << view[i].codes[1][r];
            cout<<endl;
            cout <<"条件\t：";

            for(int rr = 6;rr<view[i].codes[2].length();rr++)
              cout << view[i].codes[2][rr];
            cout<<endl;

		}

		 cout<<"数据库中索引对象及其索引属性信息如下："<<endl;
		for(int i = 0; i < index.size(); ++i)
		{
		    have = 1;
		    cout << index[i].index_name << ":" << endl;

 		    cout <<"索引属性所在表\t：";

            for(int r = 9;r<index[i].codes[0].length();r++)
                cout <<index[i].codes[0][r];
            cout<<endl;
            cout <<"索引属性\t：";

            for(int rr = 0;rr<index[i].codes[1].length();rr++)
                cout << index[i].codes[1][rr];
            cout<<endl;
            cout <<"索引类型\t：";

            for(int rrr = 0;rrr<index[i].codes[2].length();rrr++)
                cout << index[i].codes[2][rrr];
            cout<<endl;
		    //for(int j = 0; j < view[i].codes.size(); ++j)
           //     cout <<view[i].codes[j] << endl;
		  //  cout << endl;
		}

        if(have == 0) cout << "暂无数据" << endl;

		return;
	}

	string s = "help table ";
	int f = 1;
	for(int i = 0; i < s.length(); ++i)
	{
		if(op[i] != s[i])
		{
			f = 0; break;
		}
	}
	if(f == 1)
	{
		flag = 1;
		string name;
		for(int j = s.length(); j < op.length(); ++j)
            name += op[j];
        int ff = 0;
        for(int k = 0; k < table.size(); ++k)
        {
            if(table[k].table_name == name)
            {
                cout << "数据表:" << table[k].table_name << "：" << endl;
                ifstream fin;
                string tt = table[k].table_name + ".dat";
                char file[30];
                for(int i = 0; i < tt.length(); ++i)
                    file[i] = tt[i];
                file[tt.length()] = '\0';
                fin.open(file);
                string t;
                cout << "包含属性如下："<<endl;
                while(getline(fin, t))
                    cout << t << endl;
                cout << endl;
                ff = 1;
                return;
            }
        }
        if(!ff) cout << "数据库中还没有该数据表" << endl;
		return;
	}


	s = "help view ";
	f = 1;
	for(int i = 0; i < s.length(); ++i)
	{
		if(op[i] != s[i])
		{
			f = 0; break;
		}
	}
	if(f == 1)
	{
		flag = 1;
		string name;
		for(int j = s.length(); j < op.length(); ++j)
            name += op[j];
        int ff = 0;
        for(int k = 0; k < view.size(); ++k)
        {
            if(view[k].view_name == name)
            {
                cout << "帮助信息如下："<<endl;
                cout << "视图名称："<< view[k].view_name << ":" << endl;

                ifstream fin ; //
                string tt = view[k].view_name + ".dat";//
                char file[30];//
                for(int i=0;i<tt.length();i++)//
                      file[i] = tt[i];//
                file[tt.length()] = '\0';//
                fin.open(file);//
                string t;//
                while(getline(fin, t))//
                    cout << t << endl;//
                //
                ff = 1;//

                cout <<"属性\t：";
           //for(int i = 1; i < v.codes.size()-2; ++i)
               for(int r = 5;r<view[k].codes[1].length();r++)
                 cout << view[k].codes[1][r];
               cout<<endl;
               cout <<"条件\t：";

               for(int rr = 6;rr<view[k].codes[2].length();rr++)
                  cout << view[k].codes[2][rr];
               cout<<endl;

               return;
            }
        }
        if(!ff) cout << "没有该视图" << endl;


		return;

	}

     s = "help index ";
	f = 1;
	for(int i = 0; i < s.length(); ++i)
	{
		if(op[i] != s[i])
		{
			f = 0; break;
		}
	}
	if(f == 1)
	{
		flag = 1;
		string name;
		for(int j = s.length(); j < op.length(); ++j)
            name += op[j];
        int ff = 0;
        for(int k = 0; k < index.size(); ++k)
        {
            if(index[k].index_name == name)
            {
                cout << "帮助信息如下："<<endl;
                cout << "索引名称："<< index[k].index_name << ":" << endl;

                ifstream fin ; //
                string tt = index[k].index_name + ".dat";//
                char file[30];//
                for(int i=0;i<tt.length();i++)//
                      file[i] = tt[i];//
                file[tt.length()] = '\0';//
                fin.open(file);//
                string t;//
                while(getline(fin, t))//
                    cout << t << endl;//
                //
                ff = 1;//

                cout <<"索引属性所在表\t：";

                for(int r = 9;r<index[k].codes[0].length();r++)
                  cout <<index[k].codes[0][r];
                cout<<endl;
                cout <<"索引属性\t：";

                for(int rr = 0;rr<index[k].codes[1].length();rr++)
                   cout << index[k].codes[1][rr];
                cout<<endl;

                cout <<"索引类型\t：";

                for(int rrr = 0;rrr<index[k].codes[2].length();rrr++)
                   cout << index[k].codes[2][rrr];
                cout<<endl;


               return;
            }
        }
        if(!ff) cout << "没有该视图" << endl;


		return;

	}


}

void CreateTable_op(string op, int &flag)//建表
{

    string s = "create table ";
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
        flag = 2;
        CreateTable(op);
    }

    return;
}

void CreateView_op(string op, int &flag)//建立视图
{
    string s = "create view ";
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
        flag = 3;
        CreatView(op);
    }

    return;
}

void CreateIndex_op(string op, int &flag)//建立索引
{
    string s = "create index ";
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
        flag = 7;
        CreatIndex(op);
    }

    return;
}

void Insert_op(string op, int &flag)//insert插入语句
{

    string s = "insert into ";
    int f = 4;
    for(int i = 0; i < s.length(); ++i)
    {
        if(op[i] != s[i])
       {
            f = 0; break;
        }
    }

    if(f == 0)
        return;
    else
        flag = 4;
    string table_name = "";
    int i = s.length();
    for( ; i < op.length() && op[i] != ' '; ++i)
        table_name += op[i];

    int k = -1;
    for(int j = 0; j < table.size(); ++j)//
    {
        if(table[j].table_name == table_name)
        {
            k = j;
            break;
        }
    }
    if(k == -1)
    {
        cout << "插入失败，数据库中没有该表" << endl;
        flag = 0;

        return;
    }

    s = "values(";
    ++i;
    for(int j=0; j < s.length(); ++j, ++i)
    {
        if(op[i] != s[j])
        {
            cout << "插入语句格式错误" << endl;
            flag = 0;
            return;
        }
    }

    vector<string> vec;
    string tt = "";
    while(i < op.length())
    {
        if(op[i]==',' || op[i]==')')
        {
            vec.push_back(tt);
            tt = "";
        }
        else if(op[i]=='\'' || op[i]==' ')
            i;
        else
            tt += op[i];
        ++i;
    }

    if(vec.size() != table[k].col_name.size())
    {
        cout << "插入的元组与该表结构不符" << endl;
        flag = 0;
        return;
    }

    ofstream fout;
    tt = table[k].table_name + ".txt";
    char file[30];
    for(int i = 0; i < tt.length(); ++i)
        file[i] = tt[i];
    file[tt.length()] = '\0';
    fout.open(file, ios::app);
    for(int i = 0; i < vec.size()-1; ++i)
        fout << vec[i] << " ";
    fout << vec[vec.size()-1] << endl;

    cout<<"语句解析：";
    cout<<"将元组记录（";
        for(int i = 0; i < vec.size()-1; ++i)
            if (i!=(vec.size()-1))
        {
        cout << vec[i] << ",";
        }
        else
                cout << vec[i];

    cout<<")插入数据表"<<table_name<<endl;
    fout.close();

    cout << "insert into 语句执行成功~" << endl;

    return;

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

void Delete_op(string op, int& flag)//删除语句
{

    string s = "delete";
    int i = 0;
    for( ; i < s.length(); ++i)
    {
        if(s[i]!=op[i])
            return;
    }

    flag = 5;

    getline(cin, op);
    s = "from ";
    i = 0;
    for( ; i < s.length(); ++i)
    {
        if(s[i]!=op[i])
        {
            cout << "delete 语句语法错误"  << endl;
            return;
        }
    }

    string name = "";
    for( ; i<op.length(); ++i)
        name += op[i];//表名
    int k = -1;
    for(int j = 0; j < table.size(); ++j)
    {
        if(table[j].table_name == name)
        {
            k = j; break;
        }
    }
    if(k == -1)
    {
        getline(cin, op);
        cout << "数据库中不存在该表"  << endl;
        return;
    }

    getline(cin, op);
    s = "where ";
    i = 0;
    for( ; i < s.length(); ++i)
    {
        if(s[i]!=op[i])
        {
            cout << "delete 语句语法错误"  << endl;
            return;
        }
    }

    string t = "";
    for( ; i < op.length(); ++i)
       {

        t+= op[i];
       }
       cout<<"语句解析："<<endl;
       cout<<"将数据表"<<name<<"中符合";
        cout<<t<<"条件的记录删除"<<endl;


        cout<<"delete 语句执行成功"<<endl;

}


void Update_op(string op, int& flag)//更新语句
{
    string s = "update ";
    int i = 0;
    for( ; i < s.length(); ++i)
    {
        if(s[i] != op[i])
            return;
    }

    flag = 6;
    string name = "";
    for( ; i < op.length(); ++i)
        name += op[i];
    int k=-1;
    for(int j=0; j < table.size(); ++i)
    {
        if(table[j].table_name == name)
        {
            k = j; break;
        }
    }
    if(k == -1)
    {
        cout << "更新失败，数据库中不存在该表" << endl;
        return;
    }

    string sett = "";
    getline(cin, op);
    s = "set ";
    i = 0;
    for(int j = 0; j < s.length(); ++j, ++i)
    {
        if(s[j] != op[i])
        {
            cout << "update语句语法格式错误" << endl;
            return;
        }
    }
    for( ; i < op.length(); ++i)
        sett += op[i];

    string where = "";
    getline(cin, op);
    s = "where ";
    i = 0;
    for(int j = 0; j < s.length(); ++j, ++i)
    {
        if(s[j] != op[i])
        {
            cout << "update语句格式错误" << endl;
            return;
        }
    }

    for( ; i < op.length(); ++i)
        where += op[i];
    cout <<"语句解析："<<endl;
    cout << "修改数据表"  << table[k].table_name << "中符合" << where << "条件的元组" << sett <<endl;
    cout<<"update语句执行成功~"<<endl;

}

//读txt文件
void readTxt(string file)
{
    ifstream infile;
    infile.open(file.data());   //将文件流对象与文件连接起来
   // assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行

    string s;
    while(getline(infile,s))
    {
        cout<<s<<endl;
    }
    infile.close();             //关闭文件输入流
}


void Select_op(string op, int& flag)//选择语句
{
    string s = "select ";
    int i = 0;
    for( ; i < s.length(); ++i)
    {
        if(s[i] != op[i])
            return;
    }

    flag = 7;
    string col_name = "";
    for( ; i < op.length(); ++i)
        col_name += op[i];

    string table_name="";
    getline(cin, op);
    s = "from ";
    i = 0;
    for( ; i < s.length(); ++i)
    {
        if(s[i]!=op[i])
        {
            cout << "select语句格式错误" << endl;
            return;
        }
    }

    for( ; i < op.length(); ++i)
        table_name += op[i];
    int k = -1;
    for(int j = 0; j < table.size(); ++j)
    {
        if(table[j].table_name == table_name)
        {
            k = j;
            break;
        }
    }
    if(k==-1)
    {
        cout << "select失败，不存在该表" << endl;
        return;
    }

    string where = "";
    getline(cin, op);
    s = "where ";
    i = 0;
    for( ; i < s.length(); ++i)
    {
        if(s[i]!=op[i])
        {
            cout << "select语句格式错误" << endl;
            return;
        }
    }

    for( ; i < op.length(); ++i)
        where += op[i];
    cout <<"语句解析："<<endl;
    cout << "从" << table_name << "表中查询符合条件" << where << "的元组中的属性" << col_name << endl;

    cout<<"查询到符合条件的元组如下："<<endl;
    string st = table_name+".txt";
   //string sd = table_name+".dat";

    ifstream infile;
    infile.open(st.data());   //将文件流对象与文件连接起来

    string ss;
    while(getline(infile,ss))
    {

        cout<<ss<<endl;
    }
    infile.close();
    //readTxt(st);

    cout<<"select语句执行成功~"<<endl;
}

int Login(){

	string name;
	string password;
	USER user;//用户

    char ch;
    //loop:
    while(true) //循环体
    {
        cin >>ch;
        if(ch == '1')
            break; //ch为1时，直接退出
        else if(ch == '2')
        {
            CreateUser();
            cout<<"创建新用户成功"<<endl;
            break;
        }
        else
        {
            cout<<"您的输入有误，请重新输入……"<<endl;
            f=1;
            Start();
        }
    }
	while(true)
	{
		printf("登录名:");
		cin >> name;

		printf("登录密码:");
		cin >> password;

        cout<<"---------------------------------------------------"<<endl;
		USER u = UserCompare(name, password);//输入用户与已注册用户比较
		//uu=u;

		if(u.name != name)
		{
		    cout<<"用户名或密码错误请重新输入" << endl;
		    continue;
		}
		printf("\n");

		cout<<"Welcome User [" << u.name <<"] Login MY DBMS"<< endl;
        printf("\n");
		int k = -1;

		if(u.right_super=="true")
            cout <<"您为本系统的超级用户" << endl;
        else if(u.right_ordinary=="true")
             cout <<"您为本系统的普通用户" << endl;

        break;
	}

    cout<<"---------------------------------------------------"<<endl;
    cout << "用户登录成功,请输入SQL语句:" << endl;
    return 0;

}

int main()
{
    Start(); //初始界面
    Login();//用户注册or登录
    string s;
    while(true)
    {
        getline(cin, s);
        if(s.length()==0)
            continue;
        if(s=="exit")
        {
            cout<<"成功退出系统！"<<endl;
            break;
        }
        int flag = 0;
        Help_op(s, flag);//帮助函数
        CreateTable_op(s, flag);//建表函数
        CreateView_op(s, flag);//建立视图函数
        CreateIndex_op(s,flag);
        Insert_op(s, flag);//插入函数
        Delete_op(s, flag);//删除函数
        Update_op(s, flag);//更新函数
        Select_op(s, flag);//选择函数
        CreateGrant_op(s,flag);//授权函数
        CreateRevoke_op(s,flag);//回收函数

        if(flag == 0)
            cout << "输入的SQL语句有误，请重新输入" << endl;
        else
            cout << "请输入SQL语句" << endl;
        }
	return 0;
}


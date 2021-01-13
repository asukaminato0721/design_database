#include<bits/stdc++.h>
//0-98,

using namespace std;
static int f = 0;
//��ʼ������ʾ��
int Start()
{
      if(f==0){
         cout<<" *------------------------------------------------*"<<endl;
         cout<<" |                                                | "<<endl;
         cout<<" |                  DBMS������                    | "<<endl;
         cout<<" |                                                | "<<endl;
         cout<<" *------------------------------------------------*" <<endl;
         cout<<"[�û���¼�밴1,�û�ע���밴2,�˳�ϵͳ�밴"<<"'exit']"<<endl;
      }
     else
       ;
}

class TABLE
{
//������ݽṹ
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
 //��ͼ�����ݽṹ
public:
    vector<string> codes;
    string view_name;

    VIEW() { }
};

class INDEX
{
 //���������ݽṹ
public:
    vector<string> codes;
    string index_name;

    INDEX() { }
};

class USER
{
    //�û����ݽṹ
public:
	string name;
    string password;
	string right_super;
	string right_ordinary;

	string right_select;
	string right_insert;
	string right_delete;
	string right_update;

   //�����û����������дȨ��
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
 //���������ݽṹ
public:
    vector<string> codes;
    string taken_name;

    TOKEN() { }
};



//����ȡ�����ַ������ļ��е�һ�У�������User�ĸ�����
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


// ��ȡ�ļ����Ƚϣ���ͬ�򷵻ظ��û������򷵻ؿ��û�
USER UserCompare(string name, string password)
{
    USER temp;
    ifstream fin;
    fin.open("user.dat");//���ļ�
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

//�����û�
void CreateUser()
{
    USER u;
    string name,password;
    printf("������ע���û���:");
    cin >> name;
    printf("\n");
    printf("������ע������:");
    cin >> password;
    printf("\n");
    cout<<"-----------------------"<<endl;
    string privileges;
    cout << "�������û������ͣ��������û�:s  ��ͨ�û�:o  �����ִ�Сд��" << endl;
    cin >> privileges;
    printf("\n");
    u.name = name;
    u.password = password;
    //�����дȨ��
    for(int i = 0; i < privileges.length(); ++i){
         if(privileges[i]=='s'||privileges[i]=='S')
            u.right_super = "true";
         else if(privileges[i]=='o'||privileges[i]=='O')
            u.right_ordinary = "true";
    }

   //�ļ���������������������Զ�����
    ofstream fout;
    fout.open("user.dat", ios::app);
    fout << u.name << " " << u.password << " " << u.right_super << " " << u.right_ordinary << "\n" << endl;
    fout.close();
}

//������
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
            cout << "create����ʽ���󡭡�" << endl;
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
            cout << "create����ʽ����" << endl;
            return;
        }

    }

    table.push_back(t);

    ofstream fout;
    fout.open("table.dat", ios::app);
    cout<<"�������ݱ�"<<t.table_name<<",������������������������ʾ��"<<endl;

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

    cout << "�������ݱ�"+t.table_name <<"�ɹ�~"<< endl;

}

//������ͼ����
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

    cout<<"������ͼ"<<v.view_name<<",�����Լ��������£�"<<endl;
   // ofstream fout;
    //fout.open("view.dat", ios::app);
    cout <<"��ͼ��\t��" <<v.view_name << endl;
    cout <<"����\t��";
    //for(int i = 1; i < v.codes.size()-2; ++i)
    for(int r = 5;r<v.codes[1].length();r++)
       cout << v.codes[1][r];
    cout<<endl;
    cout <<"����\t��";

    for(int rr = 6;rr<v.codes[2].length();rr++)
       cout << v.codes[2][rr];
    cout<<endl;

   ofstream outdata;
   outdata.open("view.dat",ios::app);//ios::app��β��׷�ӵ���˼
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

    cout << "������ͼ�ɹ�" << endl;
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
    cout<<"��������"<<idx.index_name<<",����Ϣ���£�"<<endl;
    ofstream fout;
    fout.open("index.dat", ios::app);
    fout<<idx.index_name<<endl;
    cout <<"������\t��" <<idx.index_name << endl;
    cout <<"�����������ڱ�\t��";
    //for(int i = 1; i < v.codes.size()-2; ++i)
    for(int r = 9;r<idx.codes[0].length();r++)
       cout <<idx.codes[0][r];
    cout<<endl;
    cout <<"��������\t��";

    for(int rr = 0;rr<idx.codes[1].length();rr++)
       cout << idx.codes[1][rr];
    cout<<endl;
   //  cout <<"��������\t��";
    cout <<"��������\t��";
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

    cout << "���������ɹ�" << endl;
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
    cout<<"��Ȩ��"<<tok.taken_name<<"����Ȩ��Ϣ���£�"<<endl;
    ofstream fout;
    fout.open("token.txt", ios::app);
    fout<<tok.taken_name<<"[";
    for(int r = 3;r<tok.codes[0].length();r++)
       fout<<tok.codes[0][r];
    fout<<"] To ";
    for(int rr = 3;rr<tok.codes[1].length();rr++)
       fout<< tok.codes[1][rr];
    fout<<endl;

    cout <<"Ȩ������   ��\t" <<tok.taken_name << endl;
    cout <<"������Ȩ����Զ���\t��";
    //for(int i = 1; i < v.codes.size()-2; ++i)
    for(int r = 3;r<tok.codes[0].length();r++)
       cout <<tok.codes[0][r];
    cout<<endl;
    cout <<"����������û�\t ��";

    for(int rr = 3;rr<tok.codes[1].length();rr++)
       cout << tok.codes[1][rr];
    cout<<endl;
    fout.close();

    cout << "��Ȩ�ɹ���" << endl;
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
    cout<<"��Ȩ��"<<tok.taken_name<<"�Ļ�����Ϣ���£�"<<endl;
    ofstream fout;
    fout.open("token.txt", ios::app);
    fout<<tok.taken_name<<"[";
    for(int r = 3;r<tok.codes[0].length();r++)
       fout<<tok.codes[0][r];
    fout<<"] From ";
    for(int rr = 5;rr<tok.codes[1].length();rr++)
       fout<< tok.codes[1][rr];
    fout<<endl;

    cout <<"Ȩ������              \t ��" <<tok.taken_name << endl;
    cout <<"������Ȩ����Զ��� \t��";
    //for(int i = 1; i < v.codes.size()-2; ++i)
    for(int r = 3;r<tok.codes[0].length();r++)
       cout <<tok.codes[0][r];
    cout<<endl;
    cout <<"��Դ�û������ղ�������\t ��";

    for(int rr = 5;rr<tok.codes[1].length();rr++)
       cout << tok.codes[1][rr];
    cout<<endl;

    fout.close();

    cout << "���ճɹ���" << endl;
}

USER uu;
void Help_op(string op, int& flag)//��ӦΪhelp���
{

	if(op == "help database")
	{

	    int have = 0;
		flag = 1;
		cout<<"���ݿ������ݱ���������ԡ������������£�"<<endl;
		for(int i = 0; i < table.size(); ++i)
		{
		    have = 1;
		    cout <<table[i].table_name<<"��" << endl;
		    for(int j = 0; j < table[i].col_name.size(); ++j)
                cout << "����"<<":\t"<<table[i].col_name[j] << " \t"<<table[i].type_value[j]<<"\t"<<endl;

		    cout << endl;
		}
        cout<<"���ݿ�����ͼ���������ԡ������������£�"<<endl;
		for(int i = 0; i < view.size(); ++i)
		{
		    have = 1;
		    cout << view[i].view_name << ":" << endl;

		    cout <<"����\t��";
           //for(int i = 1; i < v.codes.size()-2; ++i)
            for(int r = 5;r<view[i].codes[1].length();r++)
            cout << view[i].codes[1][r];
            cout<<endl;
            cout <<"����\t��";

            for(int rr = 6;rr<view[i].codes[2].length();rr++)
              cout << view[i].codes[2][rr];
            cout<<endl;

		}

		 cout<<"���ݿ�����������������������Ϣ���£�"<<endl;
		for(int i = 0; i < index.size(); ++i)
		{
		    have = 1;
		    cout << index[i].index_name << ":" << endl;

 		    cout <<"�����������ڱ�\t��";

            for(int r = 9;r<index[i].codes[0].length();r++)
                cout <<index[i].codes[0][r];
            cout<<endl;
            cout <<"��������\t��";

            for(int rr = 0;rr<index[i].codes[1].length();rr++)
                cout << index[i].codes[1][rr];
            cout<<endl;
            cout <<"��������\t��";

            for(int rrr = 0;rrr<index[i].codes[2].length();rrr++)
                cout << index[i].codes[2][rrr];
            cout<<endl;
		    //for(int j = 0; j < view[i].codes.size(); ++j)
           //     cout <<view[i].codes[j] << endl;
		  //  cout << endl;
		}

        if(have == 0) cout << "��������" << endl;

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
                cout << "���ݱ�:" << table[k].table_name << "��" << endl;
                ifstream fin;
                string tt = table[k].table_name + ".dat";
                char file[30];
                for(int i = 0; i < tt.length(); ++i)
                    file[i] = tt[i];
                file[tt.length()] = '\0';
                fin.open(file);
                string t;
                cout << "�����������£�"<<endl;
                while(getline(fin, t))
                    cout << t << endl;
                cout << endl;
                ff = 1;
                return;
            }
        }
        if(!ff) cout << "���ݿ��л�û�и����ݱ�" << endl;
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
                cout << "������Ϣ���£�"<<endl;
                cout << "��ͼ���ƣ�"<< view[k].view_name << ":" << endl;

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

                cout <<"����\t��";
           //for(int i = 1; i < v.codes.size()-2; ++i)
               for(int r = 5;r<view[k].codes[1].length();r++)
                 cout << view[k].codes[1][r];
               cout<<endl;
               cout <<"����\t��";

               for(int rr = 6;rr<view[k].codes[2].length();rr++)
                  cout << view[k].codes[2][rr];
               cout<<endl;

               return;
            }
        }
        if(!ff) cout << "û�и���ͼ" << endl;


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
                cout << "������Ϣ���£�"<<endl;
                cout << "�������ƣ�"<< index[k].index_name << ":" << endl;

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

                cout <<"�����������ڱ�\t��";

                for(int r = 9;r<index[k].codes[0].length();r++)
                  cout <<index[k].codes[0][r];
                cout<<endl;
                cout <<"��������\t��";

                for(int rr = 0;rr<index[k].codes[1].length();rr++)
                   cout << index[k].codes[1][rr];
                cout<<endl;

                cout <<"��������\t��";

                for(int rrr = 0;rrr<index[k].codes[2].length();rrr++)
                   cout << index[k].codes[2][rrr];
                cout<<endl;


               return;
            }
        }
        if(!ff) cout << "û�и���ͼ" << endl;


		return;

	}


}

void CreateTable_op(string op, int &flag)//����
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

void CreateView_op(string op, int &flag)//������ͼ
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

void CreateIndex_op(string op, int &flag)//��������
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

void Insert_op(string op, int &flag)//insert�������
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
        cout << "����ʧ�ܣ����ݿ���û�иñ�" << endl;
        flag = 0;

        return;
    }

    s = "values(";
    ++i;
    for(int j=0; j < s.length(); ++j, ++i)
    {
        if(op[i] != s[j])
        {
            cout << "��������ʽ����" << endl;
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
        cout << "�����Ԫ����ñ�ṹ����" << endl;
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

    cout<<"��������";
    cout<<"��Ԫ���¼��";
        for(int i = 0; i < vec.size()-1; ++i)
            if (i!=(vec.size()-1))
        {
        cout << vec[i] << ",";
        }
        else
                cout << vec[i];

    cout<<")�������ݱ�"<<table_name<<endl;
    fout.close();

    cout << "insert into ���ִ�гɹ�~" << endl;

    return;

}


void CreateGrant_op(string op, int &flag)//������Ȩ
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

void CreateRevoke_op(string op, int &flag)//����������Ȩ
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

void Delete_op(string op, int& flag)//ɾ�����
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
            cout << "delete ����﷨����"  << endl;
            return;
        }
    }

    string name = "";
    for( ; i<op.length(); ++i)
        name += op[i];//����
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
        cout << "���ݿ��в����ڸñ�"  << endl;
        return;
    }

    getline(cin, op);
    s = "where ";
    i = 0;
    for( ; i < s.length(); ++i)
    {
        if(s[i]!=op[i])
        {
            cout << "delete ����﷨����"  << endl;
            return;
        }
    }

    string t = "";
    for( ; i < op.length(); ++i)
       {

        t+= op[i];
       }
       cout<<"��������"<<endl;
       cout<<"�����ݱ�"<<name<<"�з���";
        cout<<t<<"�����ļ�¼ɾ��"<<endl;


        cout<<"delete ���ִ�гɹ�"<<endl;

}


void Update_op(string op, int& flag)//�������
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
        cout << "����ʧ�ܣ����ݿ��в����ڸñ�" << endl;
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
            cout << "update����﷨��ʽ����" << endl;
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
            cout << "update����ʽ����" << endl;
            return;
        }
    }

    for( ; i < op.length(); ++i)
        where += op[i];
    cout <<"��������"<<endl;
    cout << "�޸����ݱ�"  << table[k].table_name << "�з���" << where << "������Ԫ��" << sett <<endl;
    cout<<"update���ִ�гɹ�~"<<endl;

}

//��txt�ļ�
void readTxt(string file)
{
    ifstream infile;
    infile.open(file.data());   //���ļ����������ļ���������
   // assert(infile.is_open());   //��ʧ��,�����������Ϣ,����ֹ��������

    string s;
    while(getline(infile,s))
    {
        cout<<s<<endl;
    }
    infile.close();             //�ر��ļ�������
}


void Select_op(string op, int& flag)//ѡ�����
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
            cout << "select����ʽ����" << endl;
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
        cout << "selectʧ�ܣ������ڸñ�" << endl;
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
            cout << "select����ʽ����" << endl;
            return;
        }
    }

    for( ; i < op.length(); ++i)
        where += op[i];
    cout <<"��������"<<endl;
    cout << "��" << table_name << "���в�ѯ��������" << where << "��Ԫ���е�����" << col_name << endl;

    cout<<"��ѯ������������Ԫ�����£�"<<endl;
    string st = table_name+".txt";
   //string sd = table_name+".dat";

    ifstream infile;
    infile.open(st.data());   //���ļ����������ļ���������

    string ss;
    while(getline(infile,ss))
    {

        cout<<ss<<endl;
    }
    infile.close();
    //readTxt(st);

    cout<<"select���ִ�гɹ�~"<<endl;
}

int Login(){

	string name;
	string password;
	USER user;//�û�

    char ch;
    //loop:
    while(true) //ѭ����
    {
        cin >>ch;
        if(ch == '1')
            break; //chΪ1ʱ��ֱ���˳�
        else if(ch == '2')
        {
            CreateUser();
            cout<<"�������û��ɹ�"<<endl;
            break;
        }
        else
        {
            cout<<"���������������������롭��"<<endl;
            f=1;
            Start();
        }
    }
	while(true)
	{
		printf("��¼��:");
		cin >> name;

		printf("��¼����:");
		cin >> password;

        cout<<"---------------------------------------------------"<<endl;
		USER u = UserCompare(name, password);//�����û�����ע���û��Ƚ�
		//uu=u;

		if(u.name != name)
		{
		    cout<<"�û����������������������" << endl;
		    continue;
		}
		printf("\n");

		cout<<"Welcome User [" << u.name <<"] Login MY DBMS"<< endl;
        printf("\n");
		int k = -1;

		if(u.right_super=="true")
            cout <<"��Ϊ��ϵͳ�ĳ����û�" << endl;
        else if(u.right_ordinary=="true")
             cout <<"��Ϊ��ϵͳ����ͨ�û�" << endl;

        break;
	}

    cout<<"---------------------------------------------------"<<endl;
    cout << "�û���¼�ɹ�,������SQL���:" << endl;
    return 0;

}

int main()
{
    Start(); //��ʼ����
    Login();//�û�ע��or��¼
    string s;
    while(true)
    {
        getline(cin, s);
        if(s.length()==0)
            continue;
        if(s=="exit")
        {
            cout<<"�ɹ��˳�ϵͳ��"<<endl;
            break;
        }
        int flag = 0;
        Help_op(s, flag);//��������
        CreateTable_op(s, flag);//������
        CreateView_op(s, flag);//������ͼ����
        CreateIndex_op(s,flag);
        Insert_op(s, flag);//���뺯��
        Delete_op(s, flag);//ɾ������
        Update_op(s, flag);//���º���
        Select_op(s, flag);//ѡ����
        CreateGrant_op(s,flag);//��Ȩ����
        CreateRevoke_op(s,flag);//���պ���

        if(flag == 0)
            cout << "�����SQL�����������������" << endl;
        else
            cout << "������SQL���" << endl;
        }
	return 0;
}


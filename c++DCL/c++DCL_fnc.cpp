#include<bits/stdc++.h>

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

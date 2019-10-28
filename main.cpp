#include <iostream>
#include<fstream>
#include<algorithm>
#include<sstream>
#include<windows.h>
#include<string>
#include<string.h>
#include<conio.h>
#include<dos.h>
#include<stdio.h>
#include<limits>
#include<ctime>
#include <stack>

using namespace std;

void gotoxy(short x,short y);
void menu();
void product_menu();
void productNew();
void productEdit();
void productDelete();
void productDisplay();
void cashier();
void new_order();
void transaction_history();

void user_menu();
void userNew();
void userSearchEditRemove();
void userList();

// ================== PRODUCT CLASS ============== //
class Product {
    public:
        string id;
        string name;
        double price;

        Product() {
            this->price = -1;
        }

        string timeStamp()
        {
            time_t rawtime;
            struct tm * timeinfo;
            char buffer[80];

            time (&rawtime);
            timeinfo = localtime(&rawtime);

            strftime(buffer,sizeof(buffer),"%d-%m-%Y %H:%M:%S",timeinfo);
            std::string str(buffer);

            return str;
        }

        string spaceToUnderscore(string str)
        {
            replace(str.begin(),str.end(),' ','_');
            return str;
        }

        string underscore2Space(string str)
        {
            replace(str.begin(),str.end(),'_',' ');
            return str;
        }

        string productID(string letter)
        {
            int num;
            srand(time(NULL));
            num = rand() % 9000 + 1000;
            stringstream ss;
            ss << num;
            string s = ss.str();
            return letter + "-" + s;
        }

        bool create(string name, double price) {
            string id = productID("IT");

            ofstream myfile("product.txt",ios::out | ios::app | ios::binary);
            myfile.seekp(0,ios::beg);

            if(myfile.is_open())
            {
                myfile<<id<<" "<<spaceToUnderscore(name)<<" "<<price<<endl;
                myfile.close();
                return true;

            }
            return false;
        }

        void fetchAll() {
            ifstream myfile("product.txt");

            string tempId,tempName,tempPrice;
                cout<<"   Item Code | Name | Price"<<endl;
                cout<<"-------------------------------"<<endl;
            while(myfile>>tempId>>tempName>>tempPrice) {
                cout<<"   "<<tempId<<" | "<<underscore2Space(tempName)<<" | P"<<tempPrice<<endl;
            }
        }

        void fetchAllCenter() {
            ifstream myfile("product.txt");

            string tempId,tempName,tempPrice;
                cout<<"                                              Item Code | Name | Price"<<endl;
                cout<<"                                            -------------------------------"<<endl;
            while(myfile>>tempId>>tempName>>tempPrice) {
                cout<<"                                              "<<tempId<<" | "<<underscore2Space(tempName)<<" | P"<<tempPrice<<endl;
            }
        }

        Product fetch(string id)
        {
            Product p;
            ifstream myfile("product.txt");
            string tempId,tempName;
            double tempPrice;
            while(myfile>>tempId>>tempName>>tempPrice) {
                if(tempId == id) {
                    p.id = tempId;
                    p.name = tempName;
                    p.price = tempPrice;
                    break;
                }
            }
            return p;
        }

        void update(string id,string name,double price)
        {
            ifstream filein("product.txt");
            ofstream fileout("temp_product.txt");

            if(!filein || !fileout)
            {
                cout<<"unable to update file";
            }
            else
            {
                string tempId,tempName;
                double tempPrice;
                while(filein>>tempId>>tempName>>tempPrice)
                {
                    if(id == tempId)
                    {
                        fileout<<tempId<<" "<<spaceToUnderscore(name)<<" "<<price<<endl;
                    }
                    else
                    {
                        fileout<<tempId<<" "<<tempName<<" "<<tempPrice<<endl;
                    }
                }
            }
            filein.close();
            fileout.close();

            ofstream out("product.txt");
            ifstream in("temp_product.txt");

            out << in.rdbuf();
        }

        void destroy(string id)
        {
            ifstream filein("product.txt");
            ofstream fileout("temp_product.txt");

            if(!filein || !fileout)
            {
                cout<<"unable to update file";
            }
            else
            {
                string tempId,tempName;
                double tempPrice;
                while(filein>>tempId>>tempName>>tempPrice)
                {
                    if(id == tempId)
                    {
                        continue;
                    }
                    else
                    {
                        fileout<<tempId<<" "<<tempName<<" "<<tempPrice<<endl;
                    }
                }
            }
            filein.close();
            fileout.close();

            ofstream out("product.txt");
            ifstream in("temp_product.txt");

            out << in.rdbuf();
        }
    private:
        //private variables and methods here
};

// ============== TRANSACTION CLASS ============= //
class Transaction: Product{
    public:
        string id;
        string user_id;
        double amount;

        Transaction create(string fullname,double amount) {
            Transaction obj;
            string id = productID("T");
            obj.id = id;
            string stamp = timeStamp();
            ofstream myfile("transaction.txt",ios::out | ios::app | ios::binary);
            myfile.seekp(0,ios::beg);

            if(myfile.is_open())
            {
                myfile<<id<<" "<<fullname<<" "<<amount<<" "<<spaceToUnderscore(timeStamp())<<endl;
                myfile.close();
            }
            return obj;
        }

        void fetchAll() {
            ifstream myfile("transaction.txt");
            string tempId,tempUserId,tempTimestamp;
            double tempAmount;
                    cout<<"                                          OR No. | Cashier Name | Amount | Timestamp"<<endl;
            if(myfile.is_open()) {
                while(myfile>>tempId>>tempUserId>>tempAmount>>tempTimestamp) {
                    cout<<"                                          "<<tempId<<" | "<<tempUserId<<" | P"<<tempAmount<<" | "<<underscore2Space(tempTimestamp)<<endl;
                }
            }
        }

};

class User : Product {
    public:
        string id;
        string fullname;
        string username;
        string password;

        void create(string fullname,string username,string password)
        {
            string id = productID("USR");
            ofstream myfile("user.txt",ios::out | ios::app | ios::binary);
            myfile.seekp(0,ios::beg);

            if(myfile.is_open()) {
                myfile<<id<<" "<<spaceToUnderscore(fullname)<<" "<<username<<" "<<password<<endl;
            }
        }

        User fetch(string username)
        {
            User obj;
            ifstream myfile("user.txt");
            string tempId,tempFullName,tempUsername,tempPassword;

            while(myfile>>tempId>>tempFullName>>tempUsername>>tempPassword) {
                if(tempUsername == username) {
                    obj.id = tempId;
                    obj.fullname = tempFullName;
                    obj.username = tempUsername;
                    obj.password = tempPassword;
                    break;
                }
            }

            return obj;
        }

        User fetchById(string id)
        {
            User obj;
            ifstream myfile("user.txt");
            string tempId,tempFullName,tempUsername,tempPassword;

            while(myfile>>tempId>>tempFullName>>tempUsername>>tempPassword) {
                if(tempId == id) {
                    obj.id = tempId;
                    obj.fullname = tempFullName;
                    obj.username = tempUsername;
                    obj.password = tempPassword;
                    break;
                }
            }

            return obj;
        }

        bool checkUserName(string username)
        {
            User obj;
            ifstream myfile("user.txt");
            string tempId,tempFullName,tempUsername,tempPassword;

            while(myfile>>tempId>>tempFullName>>tempUsername>>tempPassword) {
                if(tempUsername == username) {
                    return true;
                }
            }

            return false;
        }

        void fetchAll()
        {
            ifstream myfile("user.txt");

            string tempId,tempFullname,tempUsername, tempPassword;
                cout<<"                                           ID | Full Name | User Name | Password"<<endl;
                cout<<"                                              -------------------------------"<<endl;
            while(myfile>>tempId>>tempFullname>>tempUsername>>tempPassword) {
                cout<<"                                           "<<tempId<<" | "<<underscore2Space(tempFullname)<<" | "<<tempUsername<<" | "<<tempPassword<<endl;
            }
        }

        void update(string id,string fullname,string password)
        {
            ifstream filein("user.txt");
            ofstream fileout("temp_user.txt");

            if(!filein || !fileout)
            {
                cout<<"unable to update file";
            } else {
                string tempId,tempFullname,tempUsername,tempPassword;

                while(filein>>tempId>>tempFullname>>tempUsername>>tempPassword)
                {
                    if(id == tempId)
                    {
                        fileout<<tempId<<" "<<spaceToUnderscore(fullname)<<" "<<tempUsername<<" "<<password<<endl;
                    }
                    else
                    {
                        fileout<<tempId<<" "<<tempFullname<<" "<<tempUsername<<" "<<tempPassword<<endl;
                    }
                }
            }

            filein.close();
            fileout.close();

            ofstream out("user.txt");
            ifstream in("temp_user.txt");

            out << in.rdbuf();
        }

        void remove(string id)
        {
            ifstream filein("user.txt");
            ofstream fileout("temp_user.txt");

            if(!filein || !fileout)
            {
                cout<<"unable to update file";
            } else {
                string tempId,tempFullname,tempUsername,tempPassword;

                while(filein>>tempId>>tempFullname>>tempUsername>>tempPassword)
                {
                    if(id == tempId)
                    {
                        continue;
                    }
                    else
                    {
                        fileout<<tempId<<" "<<tempFullname<<" "<<tempUsername<<" "<<tempPassword<<endl;
                    }
                }
            }

            filein.close();
            fileout.close();

            ofstream out("user.txt");
            ifstream in("temp_user.txt");

            out << in.rdbuf();
        }

        bool login(string username,string password)
        {
            ifstream myfile("user.txt");
            bool isCorrect;

            string tempFullname,tempUsername,tempPassword,tempId;

            while(myfile>>tempId>>tempFullname>>tempUsername>>tempPassword) {
                if(username == tempUsername && password == tempPassword ) {
                    isCorrect = true;
                    break;
                } else {
                    isCorrect = false;
                }
            }
            return isCorrect;
        }
};

class Receipt: Product {
    public:
        string orders;
        double cash;
        double amount;
        double change;
        string user;
        string orNum;

        Receipt(string orders, double cash, double amount, double change,string orNum,string user) {
            this->orders = orders;
            this->cash = cash;
            this->amount = amount;
            this->change = change;
            this->user = user;
            this->orNum = orNum;
        }

        void create(){

            ofstream myfile;
            string fileName = "receipts\\" + orNum + ".txt";
            myfile.open(fileName.c_str());
            if(myfile.is_open())
            {
                myfile<<"            *** BOLIMA FASTFOOD *** "<<endl;
                myfile<<"         *** Contact No.: 888-8888 *** "<<endl;
                myfile<<"    *** Email: bolimafastfood@gmail.com *** "<<endl;
                myfile<<"           --- Official Receipt --- "<<endl<<endl;
                myfile<<" ----------------------------------------------------- "<<endl;
                myfile<<"                              Date: " <<timeStamp()<<endl<<endl;

                myfile<<"OR No. :"<<this->orNum<<endl;
                myfile<<"Order(s): "<<this->orders<<endl;
                myfile<<"Total Amount: "<<this->amount<<endl;
                myfile<<"------------------------"<<endl;
                myfile<<"Cash Received: "<<this->cash<<endl;
                myfile<<"Change: "<<this->change<<"                     Cashier: "<<underscore2Space(user);
            }
        }

};


static int TOTAL_AMOUNT = 0;
static string TOTAL_ORDERS = "";
static string USER_FULLNAME = "";
int main()
{

    system("color 2f");
    system("color 0e");

    char b = 219;

    cout<<" \n\n\n\t\t\t\t\tPlease wait, Loading...";
    cout<< "\n\n\n";
    cout<< "\t\t\t\t\t";

    for(int i = 0;i<=25;i++) {
        cout<<b;
        Sleep(200);
    }
    system("cls");

    RELOGIN:
    string username, password;
    char c = ' ';

    gotoxy(50,10);
    cout<<"USERNAME: ";

    gotoxy(50,11);
    cout<<"PASSWORD: ";

    gotoxy(60,10);
    cin>>username;

    gotoxy(60,11);
    while(c!=13) //enter key;
    {
         c=getch();
         if(c!=13)
         {
            password += c;
         }

    }

    User user;
    cout<<endl;
    if(user.login(username,password)) {
        User result = user.fetch(username);
        USER_FULLNAME = result.fullname;
        menu();
    } else {
        gotoxy(45,15);
        cout<<"Incorrect Username/Password"<<endl;

        string yesNo;

        gotoxy(50,17);
        cout<<"Try Again?(Y/N): ";
        cin>>yesNo;

        if(yesNo == "Y" || yesNo == "y") {
            system("cls");
            goto RELOGIN;
        } else {
            exit(0);
        }
    }


    //gotoxy(60,11);
    //cin>>password;
    //menu();
    //@return value = 0
    return 0;
}

void menu()
{
    system("cls");
    gotoxy(50,5);
    cout << " * * * BOLIMA FASTFOOD * * *" << endl;

    gotoxy(50,10);
    cout<<"[1] New Order"<<endl;

    gotoxy(50,11);
    cout<<"[2] Transaction History"<<endl;

    gotoxy(50,12);
    cout<<"[3] Products"<<endl;

    gotoxy(50,13);
    cout<<"[4] User Accounts"<<endl;

    gotoxy(50,14);
    cout<<"[5] Quit"<<endl;

    int choice;

    gotoxy(50,16);
    cout<<"Enter Your Choice: ";
    cin>>choice;


    switch(choice)
    {
        case 1:
            new_order();
            break;
        case 2:
            transaction_history();
            break;
        case 3:
            product_menu();
            break;
        case 4:
            user_menu();
            break;
        case 5:
            exit(0);
            break;
        default:
            gotoxy(45,18);
            cout<<"Invalid Option, System Shutdown"<<endl;
            break;
    }
}

/* ========================= PRODUCTS ========================== */
void product_menu()
{
    system("cls");

    gotoxy(50,5);
    cout << " * * * PRODUCTS * * *" << endl;

    gotoxy(50,10);
    cout<<"[1] New Product"<<endl;

    gotoxy(50,11);
    cout<<"[2] Edit Product"<<endl;

    gotoxy(50,12);
    cout<<"[3] Delete Product"<<endl;

    gotoxy(50,13);
    cout<<"[4] Product List"<<endl;

    gotoxy(50,14);
    cout<<"[5] Back"<<endl;

    int choice;

    gotoxy(50,17);
    cout<<"Enter Your Choice: ";
    cin>>choice;

    switch(choice)
    {
        case 1:
            productNew();
            break;
        case 2:
            productEdit();
            break;
        case 3:
            productDelete();
            break;
        case 4:
            productDisplay();
            break;
        case 5:
            menu();
            break;
        default:
            cout<<"Unknown Option, System Exit"<<endl;
    }
}

void productNew()
{
    system("cls");

    gotoxy(50,5);
    cout << " * * * CREATE NEW PRODUCT * * *" << endl;

    string productName;
    double productPrice;

    gotoxy(45,11);
    cout << "Product Name: ";

    gotoxy(45,13);
    cout << "Product Price: ";

    gotoxy(59,11);
    cin.ignore();
    getline(cin,productName);

    gotoxy(60,13);
    cin>>productPrice;

    Product product;
    if(product.create(productName,productPrice)) {
        gotoxy(42,15);
        cout<<"PRODUCT HAS BEEN ADDED."<<endl;

        string yesNo;

        gotoxy(35,17);
        cout<<"Do you want to add Another?(Y/N): ";
        cin>>yesNo;

        if(yesNo == "Y" || yesNo == "y") {
            productNew();
        } else {
            product_menu();
        }
    } else {
        cout<<"Product Must Be Integer"<<endl;
    }
}

void productEdit()
{
    system("cls");
    gotoxy(45,5);
    cout << " * * * SEARCH/UPDATE PRODUCT * * *" << endl;

    string id;

    gotoxy(45,7);
    cout<<"Enter Item Code: ";
    cin>>id;

    Product product;
    Product result = product.fetch(id);

    if(result.price < 0) {

        gotoxy(45,8);
        cout<<"Unknown Item Code. "<<endl;

        string yesNo;
        gotoxy(45,10);
        cout<<"Do you want to search another item?(Y/N): ";
        cin>>yesNo;

        if(yesNo == "Y" || yesNo == "y") {
            productEdit();
        } else {
            product_menu();
        }

    } else {

        gotoxy(45,9);
        cout<<"Item Code: "<<result.id<<endl;

        gotoxy(45,10);
        cout<<"Item Name: "<<result.underscore2Space(result.name)<<endl;

        gotoxy(45,11);
        cout<<"Item Price: "<<result.price<<endl;

        string yesNo;

        gotoxy(45,13);
        cout<<"Do you want to edit this Item?(Y/N):";
        cin>>yesNo;

        if(yesNo == "Y" || yesNo == "y") {
            string itemName;
            double itemPrice = 0;

            gotoxy(40,14);
            cout<<"----------------------------------------------- "<<endl;

            gotoxy(45,15);
            cout<<"Enter New Item Name: ";
            cin.ignore();
            getline(cin,itemName);

            gotoxy(45,16);
            cout<<"Enter New Item Price: ";
            cin>>itemPrice;

            Product p;

            string editYesNo;

            gotoxy(38,18);
            cout<<"Are you sure you want to edit this item?(Y/N): ";
            cin>>editYesNo;

            if(editYesNo == "Y" || editYesNo == "y") {
                p.update(id,itemName,itemPrice);

                gotoxy(42,20);
                cout<<"*** ITEM HAS BEEN UPDATED ***"<<endl;
            }

            string yesNo;

            gotoxy(38,22);
            cout<<"Do you want to search another item?(Y/N): ";
            cin>>yesNo;

            if(yesNo == "Y" || yesNo == "y") {
                productEdit();
            } else {
                product_menu();
            }

        } else {
            string yesNo;
            gotoxy(45,22);
            cout<<"Do you want to search another item?(Y/N): ";
            cin>>yesNo;

            if(yesNo == "Y" || yesNo == "y") {
                productEdit();
            } else {
                product_menu();
            }
        }
    }

}

void productDelete()
{
    system("cls");
    gotoxy(45,5);
    cout << " * * * REMOVE PRODUCT * * *" << endl;

    string id;

    gotoxy(45,7);
    cout<<"Enter Item Code: ";
    cin>>id;

    Product product;
    Product result = product.fetch(id);

    if(result.price < 0) {

        gotoxy(44,9);
        cout<<" *** Unknown Item Code *** "<<endl;

        string yesNo;
        gotoxy(38,11);
        cout<<"Do you want to search another item?(Y/N): ";
        cin>>yesNo;

        if(yesNo == "Y" || yesNo == "y") {
            productDelete();
        } else {
            product_menu();
        }
    } else {

        gotoxy(45,9);
        cout<<" *** ITEM FOUND ***"<<endl;

        gotoxy(45,10);
        cout<<"Item Code: "<<result.id<<endl;

        gotoxy(45,11);
        cout<<"Item Name: "<<result.underscore2Space(result.name)<<endl;

        gotoxy(45,12);
        cout<<"Item Price: "<<result.price<<endl;

        string deleteYesNo;

        gotoxy(45,14);
        cout<<"Do you want to remove this Item?(Y/N):";
        cin>>deleteYesNo;

        if(deleteYesNo == "Y" || deleteYesNo == "y") {
            Product p;
            p.destroy(id);

            gotoxy(42,16);
            cout<<"*** ITEM HAS BEEN REMOVED ***"<<endl;
        }

        string yesNo;
        gotoxy(38,18);
        cout<<"Do you want to search another item?(Y/N): ";
        cin>>yesNo;

        if(yesNo == "Y" || yesNo == "y") {
            productDelete();
        } else {
            menu();
        }
    }
}

void productDisplay()
{
    system("cls");
    gotoxy(45,5);
    cout << " * * * LIST PRODUCTS * * *" << endl;

    cout<<endl<<endl;
    Product product;
    product.fetchAllCenter();

    cout<<endl;
    string x;
    cout<<"                                          Type (X) and return key to exit: ";
    cin>>x;

    if(x == "X" || x=="x") {
        system("cls");
        product_menu();
    } else {
        productDisplay();
    }
}

void new_order()
{
    system("cls");


    gotoxy(5,5);
    cout << " * * * MENU LIST * * *" << endl;
    cout<<endl<<endl;
    Product p;
    p.fetchAll();

    gotoxy(50,5);
    cout << " * * * TRANSACTION * * *" << endl;

    gotoxy(85,5);
    cout<<" * * * TRANSACTION SUMMARY * * * "<<endl;

    gotoxy(85,8);
    cout<<"TOTAL AMOUNT: "<<TOTAL_AMOUNT<<endl;

    gotoxy(85,9);
    cout<<"ORDER(S): "<<TOTAL_ORDERS<<endl;



    string id;
    int qty;

    gotoxy(50,8);
    cout<<"Enter Item ID: ";
    cin>>id;

    //CONVERT STRING TO UPPERCASE
    transform(id.begin(), id.end(), id.begin(), ::toupper);

    Product product = p.fetch(id);

    if(product.price < 0) {
        gotoxy(50,10);
        cout<<"Invalid Item Code";

        string yesNo;
        gotoxy(45,17);
        cout<<"Do you want to add another Item?(Y/N):";
        cin>>yesNo;

        if(yesNo == "Y" || yesNo == "y") {
            new_order();
        } else {
            menu();
        }

    } else {
        gotoxy(50,10);
        cout<<"You Choose: "<<product.name<<endl;

        gotoxy(50,11);
        cout<<"Price: "<<product.price<<endl;

        gotoxy(50,12);
        cout<<"-------------------------"<<endl;
        gotoxy(50,13);
        cout<<"Enter Quantity: ";
        cin>>qty;

        double sum;

        sum = product.price * qty;
        stringstream ss;
        ss << qty;
        TOTAL_AMOUNT += sum;
        TOTAL_ORDERS += "(" + ss.str() + ")" + product.name + ",";

        gotoxy(50,14);
        cout<<"-------------------------"<<endl;

        gotoxy(50,15);
        cout<<"Total: "<<sum;

        string yesNo;
        gotoxy(45,17);
        cout<<"Do you want to add another Item?(Y/N):";
        cin>>yesNo;

        if(yesNo == "Y" || yesNo == "y") {
            new_order();
        } else {
            system("cls");
            cashier();
        }
    }

}

void cashier()
{
    system("cls");


    gotoxy(50,5);
    cout << " * * * CASHIER PAYMENT * * *" << endl;;

    gotoxy(50,8);
    cout<<"Customer Order(s): "<<TOTAL_ORDERS<<endl;

    gotoxy(50,10);
    cout<<"Total Amount: "<<TOTAL_AMOUNT<<endl;

    double cash = 0;

    gotoxy(50,11);
    cout<<"------------------"<<endl;
    gotoxy(50,13);
    cout<<"Enter Total Cash: ";
    cin>>cash;

    if(cash < TOTAL_AMOUNT) {
        gotoxy(32,15);
        cout<<"Total cash is less than the Total Amount, Transaction failed!"<<endl;

        string yesNo;
        gotoxy(45,21);
        cout<<"Do you want another Transaction?(Y/N):";
        cin>>yesNo;

        if(yesNo == "Y" || yesNo == "y") {
            TOTAL_AMOUNT = 0;
            TOTAL_ORDERS = "";
            new_order();
        } else {
            system("cls");
            TOTAL_AMOUNT = 0;
            TOTAL_ORDERS = "";
            main();
        }

    } else {
        double change = 0;
        change = cash - TOTAL_AMOUNT;

        gotoxy(50, 15);
        cout<<"The Total Change Is:"<<change<<endl;

        gotoxy(50, 18);
        cout<<" *** THANK YOU *** "<<endl;

        Transaction transaction;
        Transaction getId;
        getId = transaction.create(USER_FULLNAME,TOTAL_AMOUNT);

        gotoxy(50, 19);
        cout<<"OR No. : [ "<<getId.id<<" ]"<<endl;

        gotoxy(32, 20);
        cout<<"Please print the receipt, File location: receipts/"<<getId.id<<".txt"<<endl;

        Receipt receipt(TOTAL_ORDERS,cash,TOTAL_AMOUNT,change,getId.id,USER_FULLNAME);
        receipt.create();

        string yesNo;
        gotoxy(45,22);
        cout<<"Do you want another Transaction?(Y/N):";
        cin>>yesNo;

        if(yesNo == "Y" || yesNo == "y") {
            TOTAL_AMOUNT = 0;
            TOTAL_ORDERS = "";
            new_order();
        } else {
            system("cls");
            TOTAL_AMOUNT = 0;
            TOTAL_ORDERS = "";
            menu();
        }
    }

}

// ========== TRANSACTION HISTORY ============= //
void transaction_history()
{
    system("cls");
    gotoxy(50,5);
    cout << " * * * TRANSACTION_HISTORY * * *" << endl;;
    Transaction transaction;

    cout<<endl<<endl<<endl;
    transaction.fetchAll();

    cout<<endl;
    string x;
    cout<<"                                                Type (X) and return key to exit: ";
    cin>>x;

    if(x == "X" || x=="x") {
        system("cls");
        menu();
    } else {
        transaction_history();
    }
}

void user_menu()
{
    system("cls");

    gotoxy(50,5);
    cout << " * * * USER ACCOUNTS * * *" << endl;

    gotoxy(50,10);
    cout<<"[1] New User"<<endl;

    gotoxy(50,11);
    cout<<"[2] Search/Edit/Remove User"<<endl;

    gotoxy(50,12);
    cout<<"[3] List User"<<endl;

    gotoxy(50,13);
    cout<<"[4] Back"<<endl;

    int choice;

    gotoxy(50,17);
    cout<<"Enter Your Choice: ";
    cin>>choice;

    switch(choice)
    {
        case 1:
            userNew();
            break;
        case 2:
            userSearchEditRemove();
            break;
        case 3:
            userList();
            break;
        case 4:
            menu();
            break;
        default:
            menu();
            break;

    }
}

void userNew()
{
    system("cls");


    gotoxy(50,5);
    cout << " * * * NEW USER * * *" << endl;

    string fullName;
    string username;
    string password;

    gotoxy(50,7);
    cout<<"Enter Full Name:";

    gotoxy(50,8);
    cout<<"Enter User Name: ";

    gotoxy(50,9);
    cout<<"Enter Password : ";

    gotoxy(66,7);
    cin.ignore();
    getline(cin,fullName);

    gotoxy(66,8);
    getline(cin,username);

    gotoxy(66,9);
    getline(cin,password);

    if(fullName.empty() || username.empty() || password.empty()) {
        gotoxy(47,11);
        cout<<"All fields are required"<<endl;
    } else {
        User user;

        if(user.checkUserName(username)) {
            gotoxy(41,11);
            cout<<"Username already exists. Please try another one"<<endl;
        } else {
            User user;
            user.create(fullName,username,password);

            gotoxy(44,11);
            cout<<"User has been successfully created"<<endl;
        }
    }
    string yesNo;

    gotoxy(41,13);
    cout<<"Do you want to another another account?(Y/N):";
    cin>>yesNo;

    if(yesNo == "Y" || yesNo == "y") {
        userNew();
    } else {
        user_menu();
    }
}

void userList()
{
    system("cls");


    gotoxy(50,5);
    cout << " * * * LIST USER * * *" << endl;
    cout<<endl<<endl<<endl;

    User user;

    user.fetchAll();

    cout<<endl;
    string x;
    cout<<"                                              Type (X) and return key to exit: ";
    cin>>x;

    if(x == "X" || x=="x") {
        system("cls");
        user_menu();
    } else {
        userList();
    }
}

void userSearchEditRemove()
{
    system("cls");


    gotoxy(45,5);
    cout << " * * * SEARCH/EDIT/REMOVE USER * * *" << endl;

    string id;

    gotoxy(50,7);
    cout<<"Enter User ID:";
    cin>>id;

    if(id.empty()) {
        cout<<"Please Enter User ID"<<endl;
    } else {
        User user;
        User result = user.fetchById(id);

        if(result.id.empty()) {
            gotoxy(50,9);
            cout<<" *** USER NOT FOUND *** "<<endl;
        } else {
            gotoxy(50,9);
            cout<<" *** USER FOUND *** "<<endl;

            gotoxy(50, 10);
            cout<<"User ID: "<<result.id<<endl;

            gotoxy(50, 11);
            cout<<"Full Name: "<<result.fullname<<endl;

            gotoxy(50, 12);
            cout<<"User Name: "<<result.username<<endl;

            gotoxy(50, 13);
            cout<<"Password: "<<result.password<<endl;

            gotoxy(37,15);
            cout<<"[E - Edit ] [R - REMOVE] [S - SEARCH] [B - BACK] "<<endl;

            gotoxy(50,17);
            char choice;
            cout<<"Enter your Choice: ";
            cin>>choice;

            string fullname,password,sure;
            switch(choice)
            {
                case 'E':
                case 'e':
                    gotoxy(50,19);
                    cout<<"Enter New Full Name: ";
                    cin.ignore();
                    getline(cin,fullname);

                    gotoxy(50,20);
                    cout<<"Enter New Password: ";
                    cin>>password;

                    gotoxy(50,21);
                    cout<<"Are you sure you want to update?(Y/N):";
                    cin>>sure;

                    if(sure == "Y" || sure == "y") {
                        user.update(id,fullname,password);
                        gotoxy(40,23);
                        cout<<" *** User has been successfully updated ***";
                    } else {
                        user_menu();
                    }
                    break;
                case 'R':
                case 'r':
                    user.remove(id);
                    gotoxy(38,19);
                    cout<<" *** User has been successfully removed *** "<<endl;
                    break;
                case 'S':
                case 's':
                    cout<<"Search"<<endl;
                    userSearchEditRemove();
                case 'B':
                case 'b':
                    user_menu();
                    break;
                default:
                    menu();
                    break;
            }
        }
    }

    string yesNo;

    gotoxy(40,25);
    cout<<"Do you want to create another account?(Y/N):";
    cin>>yesNo;

    if(yesNo == "Y" || yesNo == "y") {
        userSearchEditRemove();
    } else {
        user_menu();
    }
}
/* GOTOXY */
void gotoxy(short x, short y)
{
     COORD pos ={x,y};
     SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

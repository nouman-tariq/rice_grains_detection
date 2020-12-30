#include <iostream>
#include <fstream>

using namespace std;

class account_query
{
    private:
    char account_name[10];
    char account_number[20];
    float total_balance;

    public:
    void add_details();
    void write_details();
};

void account_query::add_details()
{
    cout<<"Enter account name- ";
    cin>>account_name;
    cout<<"Enter account number- ";
    cin>>account_number;
    cout<<"Enter the total balance- ";
    cin>>total_balance;
}
void account_query::write_details()
{
    ofstream outfile;
    outfile.open("account_details.bank", ios::binary|ios::app);
    add_details();
    outfile.write(reinterpret_cast<char *>(this), sizeof(*this));
    outfile.close();    
}
int main()
{
    account_query account1;
    account1.write_details();

    
}
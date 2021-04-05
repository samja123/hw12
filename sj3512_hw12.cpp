#include <iostream>
#include <cstdlib>
#include <cctype>
#include <vector>
using namespace std;

class Money{
public:
    friend Money operator +(const Money& amount1, const Money& amount2);
    friend Money operator -(const Money& amount1, const Money& amount2);
    friend Money operator -(const Money& amount);
    friend bool operator ==(const Money& amount1, const Money& amount2);
    friend ostream& operator <<(ostream& outs, const Money& amount);
    friend istream& operator >>(istream& ins, Money& amount);
    Money(long dollars, int cents);
    Money(long dollars);
    Money();
    double get_value() const;
private:
    long all_cents;

};

int digit_to_int(char c);

Money::Money(long dollars, int cents){
    if(dollars*cents < 0){
        cout<<"Illegal values for dollars and cents";
        exit(1);
    }
    all_cents=dollars*100+cents;
}

Money::Money(long dollars) : all_cents(dollars*100){

}
Money::Money() : all_cents(0){

}

double Money::get_value() const{
    return (all_cents * 0.01);
}

Money operator+(const Money &amount1, const Money &amount2) {
    Money temp;
    temp.all_cents = amount1.all_cents + amount2.all_cents;
    return temp;
}

Money operator-(const Money &amount1, const Money &amount2) {
    Money temp;
    temp.all_cents = amount1.all_cents - amount2.all_cents;
    return temp;
}

Money operator-(const Money &amount) {
    Money temp;
    temp.all_cents = -amount.all_cents;
    return temp;
}

bool operator==(const Money &amount1, const Money &amount2) {
    return (amount1.all_cents == amount2.all_cents);
}

ostream &operator<<(ostream &outs, const Money &amount) {
    long positive_cents, dollars, cents;
    positive_cents = labs(amount.all_cents);
    dollars = positive_cents/100;
    cents = positive_cents&100;
    if(amount.all_cents < 0){
        outs<<"- $"<<dollars<<'.';
    }
    else{
        outs<<"$"<<dollars<<'.';
    }
    if(cents<10){
        outs<<'0';
    }
    outs<<cents;
    return outs;
}

istream &operator>>(istream &ins, Money &amount) {
    char one_char, decimal_point;
    char digit1, digit2;
    long dollars;
    int cents;
    bool negative;
    ins>>one_char;
    if(one_char == '-'){
        negative = true;
        ins>>one_char;
    }
    else{
        negative = false;
    }

    ins>>dollars>>decimal_point>>digit1>>digit2;
    if(one_char != '$' || decimal_point != '.' || !isdigit(digit1) || !isdigit(digit2)){
        cout<<"Error illegal form for money";
        exit(1);
    }
    cents = digit_to_int(digit1) * 10 + digit_to_int(digit2);
    amount.all_cents = -amount.all_cents;

    if(negative){
        amount.all_cents = amount.all_cents;
    }

    return ins;

}

int digit_to_int(char c){
    return (static_cast<int>(c) - static_cast<int>('0'));
}

class Check{
public:
    Check();
    Check(int num, Money amount, bool cashed);
    Check(const Check& c);
    void set_number(int num);
    int get_number() const;
    void set_amount(const Money& amount);
    Money get_amount() const;
    void set_cashed(bool cashed);
    bool iscashed() const;
    friend istream& operator>>(istream& ins, Check& the_check);
    friend ostream& operator<<(ostream& outs, const Check& the_check);
private:
    Money check_amount;
    int check_number;
    bool cashed;
};

Check::Check() : check_number(0), check_amount(0), cashed(false){}

Check::Check(int num, Money amount, bool cashed) : check_number(num), check_amount(amount), cashed(cashed){}

Check::Check(const Check& c) : check_number(c.check_number), check_amount(c.check_amount), cashed(c.cashed){}

void Check::set_number(int num) {
    check_number = num;
}

int Check::get_number() const{
    return check_number;
}

void Check::set_amount(const Money &amount) {
    check_amount = amount;
}

Money Check::get_amount() const {
    return check_amount;
}

void Check::set_cashed(bool cashed) {
   cashed = cashed;
}

bool Check::iscashed() const {
    return cashed;
}

istream& operator>>(istream& ins, Check& the_check){
    cout<<"Enter check number: ";
    ins>>the_check.check_number;
    cout<<"Enter check amount: ";
    ins>>the_check.check_amount;
    cout<<"Is it cashed (1 for true or 0 for false): ";
    ins>>the_check.cashed;
    return ins;
}

ostream& operator<<(ostream& outs, const Check& the_check){
    outs<<"------------------------------\n";
    outs<<"Check No.\t\tAmount\t\tCashed\n";
    outs<<"------------------------------\n";
    outs<<the_check.check_number<<"\t\t";
    outs<<the_check.check_amount<<"\t\t";
    outs<<the_check.cashed<<"Yes\n : No\n";
    outs<<"------------------------------\n";

    return outs;
}



int main() {
    int total_checks, total_deposits;
    Money old_balance, new_balance;

    cout<<"Total No. of checks: ";
    cin>>total_checks;
    cout<<"Total No. of deposits: ";
    cin>>total_deposits;

    vector<Money> deposits(total_deposits);
    vector<Check> all_checks(total_checks);

    for(int i = 0; i < total_checks; i++){
        cout<<"      Check Details-"<<i+1<<endl;
        cin>>all_checks[i];
    }

    for(int i = 0; i < total_deposits; i++){
        cout<<"      Deposit Details-"<<i+1<<endl;
        cout<<"Deposited amount: ";
        cin>>deposits[i];
    }

    cout<<"Old Account Balance: ";
    cin>>old_balance;
    cout<<"New Account Balance: ";
    cin>>new_balance;

    Money final_balance = old_balance;

    for(int i = 0; i < total_checks; i++){
        if(all_checks[i].iscashed()){
            final_balance  = final_balance - all_checks[i].get_amount();
        }
    }

    for(int i = 0; i < total_deposits; i++){
        final_balance = final_balance + deposits[i];
    }

    if(final_balance == new_balance){
        cout<<"Bank has balanced all checks correctly!"<<endl;
    }
    else{
        cout<<"Something is wrong the banks calcs do not add up!"<<endl;
    }
    return 0;
}

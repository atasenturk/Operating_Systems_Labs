#include <iostream>
using namespace std;
int main(void){
    int num;
    cout << "Bir sayı giriniz: ";
    cin >> num;
    int sum = 0;
    for (int i = 1; i <= num; i++)
    {
        if(i % 2 != 0){
            cout << i;
            sum += i;

            if(i != num && i != num - 1){
                cout << " + ";
            }
        }

    }
    cout << " = " << sum;
    cout << endl;
}
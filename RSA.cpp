#include <bits/stdc++.h>
using namespace std;

set<int> prime_set; // a set will be the collection of prime numbers,
                    // where we can select random primes p and q
int public_key_var;
int private_key_var;
int n_var;

// we will run the function only once to fill the set of
// prime numbers
void fill_primes()
{
    // method used to fill the primes set is sieve of
    // Eratosthenes (a method to collect prime numbers)
    vector<bool> sieve(250, true);
    sieve[0] = false;
    sieve[1] = false;
    for (int i = 2; i < 250; i++) {
        for (int j = i * 2; j < 250; j += i) {
            sieve[j] = false;
        }
    }
    // filling the prime numbers
    for (int i = 0; i < sieve.size(); i++) {
        if (sieve[i])
            prime_set.insert(i);
    }
}

// picking a random prime number and erasing that prime
// number from list because p!=q
int pick_random_prime()
{
    int k = rand() % prime_set.size();
    auto it = prime_set.begin();
    while (k--)
        it++;
    int ret = *it;
    prime_set.erase(it);
    return ret;
}

void set_keys()
{
    int prime1 = pick_random_prime(); // first prime number
    int prime2 = pick_random_prime(); // second prime number
    // to check the prime numbers selected
    // cout<<prime1<<" "<<prime2<<endl;
    n_var = prime1 * prime2;
    int phi_n = (prime1 - 1) * (prime2 - 1);
    int e = 2;
    while (1) {
        if (__gcd(e, phi_n) == 1)
            break;
        e++;
    }
    // d = (k*Φ(n) + 1) / e for some integer k
    public_key_var = e;
    int d = 2;
    while (1) {
        if ((d * e) % phi_n == 1)
            break;
        d++;
    }
    private_key_var = d;
}

// to encrypt the given number
long long int encrypt(double message)
{
    int e = public_key_var;
    long long int encrypted_text = 1;
    while (e--) {
        encrypted_text *= message;
        encrypted_text %= n_var;
    }
    return encrypted_text;
}

// to decrypt the given number
long long int decrypt(int encrypted_text)
{
    int d = private_key_var;
    long long int decrypted = 1;
    while (d--) {
        decrypted *= encrypted_text;
        decrypted %= n_var;
    }
    return decrypted;
}

// first converting each character to its ASCII value and
// then encoding it then decoding the number to get the
// ASCII and converting it to character
vector<int> encode(string message)
{
    vector<int> form;
    // calling the encrypting function in encoding function
    for (auto& letter : message)
        form.push_back(encrypt((int)letter));
    return form;
}

string decode(vector<int> encoded)
{
    string s;
    // calling the decrypting function decoding function
    for (auto& num : encoded)
        s += decrypt(num);
    return s;
}

int main()
{
    fill_primes();
    set_keys();
    string message = "Test Message";
    // uncomment below for manual input
    // cout<<"enter the message\n";getline(cin,message);
    // calling the encoding function
    vector<int> coded = encode(message);
    cout << "Initial message:\n" << message;
    cout << "\n\nThe encoded message (encrypted by public "
            "key)\n";
    for (auto& p : coded)
        cout << p;
    cout << "\n\nThe decoded message (decrypted by private "
            "key)\n";
    cout << decode(coded) << endl;
    return 0;
}

#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>

using namespace std;

template <typename T>
class matrix;

template <typename T>
bool check_for_size(matrix<T> &m1, matrix<T> &m2){
    bool answer = true;
    if (m1.get_stb() != m2.get_stb()){
        answer = false;
    }
    else if (m1.get_str() != m2.get_str()){
        answer = false;
    }
    return answer;
}

int len(double a){
    int aa = int(a);
    int t = int(a);
    int l = 0;
    while(t != a){
        a = a * 10;
        l++;
        t = int(a);
    }
    l++;
    if(aa < 0){
        aa = -aa;
        l++;
    }
    else if (aa == 0){
        return 3;
    }
    int c = 1;
    while((aa / c) > 0){
        l++;
        c = c * 10;
    }
    return l;
}
int len(int a){
    int c = 0;
    if (a == 0){
        return 1;
    }
    else if (a < 0){
        c += 1;
        a = -a;
    }
    while(a != 0){
        c += 1;
        a = a / 10; 
    }
    return c;
}
int pow(int a, int b){
    int l = 1;
    while(b != 0){
        l *= a;
        b--;
    }
    return l;
}

template <typename T>
class matrix{
  private:
    int str;
    int stb;
    T **el;
  public:
    matrix(const char namefile[]){
        ifstream fin;
        fin.open(namefile);
        if(fin.is_open()){
            fin >> str;
            fin >> stb;
            el = new T* [str];
            for(int i = 0; i < str; i++){
                el[i] = new T[stb];
            }
            for(int i = 0; i < str; i++){
                for(int j = 0; j < stb; j++){
                    fin >> el[i][j];
                }
            }
        }
        else{
            cout << "Error: reading input file " << namefile << endl;
            str = stb = 0;
            el = new T* [str];
            for(int i = 0; i < str; i++){
                el[i] = new T[stb];
            }
            for(int i = 0; i < str; i++){
                for(int j = 0; j < stb; j++){
                    el[i][j] = 0;
                }
            }
        }
        fin.close();
    }
    matrix(int a = 0, int b = 0, T c = T()){
        str = a;
        stb = b;
        el = new T* [str];
        for(int i = 0; i < str; i++){
            el[i] = new T[stb];
        }
        thread t1([&](){
            over_diagonal_updating(*this, T());
        });
        thread t2([&](){
            under_diagonal_updating(*this, T());
        });
        diagonal_updating(*this, c);
        t1.join();
        t2.join();
    }
    matrix(const matrix<T> &other){
        this->str = other.str;
        this->stb = other.stb;
        el = new T*[str];
        for(int i = 0; i < str; i++){
            el[i] = new T[stb];
        }
        for(int i = 0; i < str; i++){
            for(int j = 0; j < stb; j++){
                this->el[i][j] = other.el[i][j];
            }
        }
    }
    ~matrix(){
        for(int i = 0; i < str; i++){
            delete[] el[i];
        }
        delete[] el;
    }
    int get_stb(){
        return stb;
    }
    int get_str(){
        return str;
    }
    T *get_strok(int a){
        return el[a];
    }
    void update_strok(int a, T *s){
        for(int i = 0; i < stb; i++){
            el[a][i] = s[i];
        }
    }
    void show_matrix(){
        int max_len = 0;
        for(int i = 0; i < str; i++){
            for(int j = 0; j < stb; j++){
                if(len(el[i][j] > max_len)){
                    max_len = len(el[i][j]);
                }
            }
        }
        for(int i = 0; i < str; i++){
            for(int j = 0; j < stb; j++){
                cout.width(max_len + 1);
                cout << el[i][j] << " ";
            }
            cout << endl;
        }
    }
    void show_matrix(const char namefile[]){
        ofstream fout;
        fout.open(namefile);
        if (fout.is_open()){
            int max_len = 0;
            for(int i = 0; i < str; i++){
                for(int j = 0; j < stb; j++){
                    if(len(el[i][j] > max_len)){
                        max_len = len(el[i][j]);
                    }
                }
            }
            for(int i = 0; i < str; i++){
                for(int j = 0; j < stb; j++){
                    fout.width(max_len + 1);
                    fout << el[i][j] << " ";
                }
                fout << endl;
            }
        }
        else{
            cout << "Error: reading output file " << namefile << endl;
        }
    }
    void update_matrix(){
        cout << "Введите сначала 2 целых числа (количество строк и столбцов), а потом все элементы по порядку через пробел" << endl;
        for(int i = 0; i < str; i++){
            delete[] el[i];
        }
        delete[] el;
        cin >> str;
        cin >> stb;
        el = new T* [str];
        for(int i = 0; i < str; i++){
            el[i] = new T[stb];
        }
        for(int i = 0; i < str; i++){
            for(int j = 0; j < stb; j++){
                cin >> el[i][j];
            }
        }
    }

    bool operator == (const matrix<T> &other){
        if(this->str != other.str || this->stb != other.stb){
            return false;
        }
        bool fl1, fl2, fl3;
        thread t1([&](){
            fl1 = checking_over_diagonal(*this, other);
        });
        thread t2([&](){
            fl2 = checking_under_diagonal(*this, other);
        });
        fl3 = checking_diagonal(*this, other);
        t1.join();
        t2.join();
        return (fl1 * fl2 * fl3);
    }
    bool operator != (const matrix<T> &other){
        if(this->str != other.str || this->stb != other.stb){
            return true;
        }
        bool fl1, fl2, fl3;
        thread t1([&](){
            fl1 = r_checking_over_diagonal(*this, other);
        });
        thread t2([&](){
            fl2 = r_checking_under_diagonal(*this, other);
        });
        fl3 = r_checking_diagonal(*this, other);
        t1.join();
        t2.join();
        if((fl1 + fl2 + fl3) > 0){
            return true;
        }
        return false;
    }
    matrix<T> & operator = (const matrix<T> &other){
        for (int i = 0; i < str; i++){
                delete[] this->el[i];
            }
            delete[] this->el;
        this->str = other.str;
        this->stb = other.stb;
        el = new T*[str];
        for(int i = 0; i < str; i++){
            el[i] = new T[stb];
        }
        thread t([&](){
            for(int i = 0; i < str / 2; i++){
                for(int j = 0; j < stb; j++){
                    this->el[i][j] = other.el[i][j];
                }
            }
        });
        for(int i = str / 2; i < str; i++){
            for(int j = 0; j < stb; j++){
                this->el[i][j] = other.el[i][j];
            }
        }
        t.join();
        return *this;
    }
    bool operator == (const T a){
        if(this->stb != this->str){
            return false;
        }
        bool fl1, fl2, fl3;
        thread t1([&](){
            fl1 = checking_over_diagonal(*this, T());
        });
        thread t2([&](){
            fl2 = checking_under_diagonal(*this, T());
        });
        fl3 = checking_diagonal(*this, a);
        t1.join();
        t2.join();
        return (fl1 * fl2 * fl3);
    }
    matrix<T> operator + (const matrix<T> &other){
        matrix<T> summa(this->str, this->stb);
        if(this->str != other.str || this->stb != other.stb){
             cout << "Error: \"+\"" << endl;
             return *this;
        }
        thread tt([&](){
            summarise1(summa, *this, other);
        });
        summarise2(summa, *this, other);
        tt.join();
        return summa;
    }
    matrix<T> operator - (const matrix<T> &other){
        matrix<T> razn(this->str, this->stb);
        if(this->str != other.str || this->stb != other.stb){
             cout << "Error: \"-\"" << endl;
             return *this;
        }
        thread tt([&](){
            minusrise1(razn, *this, other);
        });
        minusrise2(razn, *this, other);
        tt.join();
        return razn;
    }
    matrix<T> operator * (const matrix<T> &other){
        matrix<T> pro(this->str, other.stb);
        int a = this->stb;
        int c;
        if (this->stb != other.str){
            cout << "Error: \"*\"" << endl;
        }
        else{
            for(int i = 0; i < pro.str; i++){
                for(int j = 0; j < pro.stb; j++){
                    c = 0;
                    for(int t = 0; t < a; t++){
                        c += this->el[i][t] * other.el[t][j];
                    }
                    pro.el[i][j] = c;
                }
            }
        }
        return pro;
    }
    matrix<T> operator * (const T a){
        matrix<T> um = *this;
        thread t([&](){
            umnrise1(um, a);
        });
        umnrise2(um, a);
        t.join();
        return um;
    }
    matrix<T> operator / (const T a){
        matrix<T> del = *this;
        thread t([&](){
            delrise1(del, a);
        });
        delrise2(del, a);
        t.join();
        return del;
    }
    matrix<T> matrix_without_strok(int number){
        matrix<T> mmm = *this;
        mmm.delete_strok_in(number);
        return mmm;
    }
    matrix<T> matrix_without_stolb(int number){
        matrix<T> mmm = *this;
        mmm.delete_stolb_in(number);
        return mmm;
    }
    T determinant(){
        if( this->stb != this->str){
            cout << "Error, matrix is not square" << endl;
            return 0;
        }
        else if(this->str == 1){
            return this->el[0][0];
        }
        else if(this->str == 2){
            return (this->el[0][0] * this->el[1][1]) - (this->el[1][0] * this->el[0][1]);
        }
        else{
            T d = T();
            thread t([&](){
                for(int i = this->get_stb() / 2; i < this->get_stb(); i++){
                    d += pow((-1), i) * this->el[0][i] * ((this->matrix_without_strok(1)).matrix_without_stolb(i + 1)).determinant();
                }
            });
            for(int i = 0; i < this->get_stb() / 2; i++){
                d += pow((-1), i) * this->el[0][i] * ((this->matrix_without_strok(1)).matrix_without_stolb(i + 1)).determinant();
            }
            t.join();
            return d;
        }
        return T();
    }
    matrix<T> operator ! (){
        if(this->get_str() != this->get_stb()){
            cout << "Error \"Determinant\", matrix must be squared" << endl;
            return *this;
        }
        T c = this->determinant();
        if (c == 0){
            matrix<T> m0(this->get_str(), this->get_stb());
            cout << "Error \"Determinant\", matrix's determinant must be not 0" << endl;
            return m0;
        }
        matrix<T> mm = this->alg_dop_matrization();
        mm.transposing();
        mm = mm / c;
        return mm;
    }
    static matrix<T> null_matrix(int a){
        matrix<T> m0(a, a);
        return m0;
    }
    static matrix<T> single_matrix(int a){
        matrix<T> m0(a, a, 1.0);
        return m0;
    }
  private:
    void delete_strok_in(int number){
        number--;
        for(int i = number; i < this->get_str() - 1; i++){
            this->update_strok(i, this->get_strok(i + 1));
        }
        delete this->el[this->get_str() - 1];
        this->str = this->get_str() - 1;
    }
    void delete_stolb_in(int number){
        number--;
        for(int i = 0; i < this->get_str(); i++){
            for(int j = number; j < this->get_stb() - 1; j++){
                this->el[i][j] = this->el[i][j + 1];
            }
        }
        this->stb = this->get_stb() - 1;
    }
    T alg_dop(int a, int b){
        return (pow((-1), a + b) * (this->matrix_without_strok(a).matrix_without_stolb(b)).determinant());
    }
    matrix<T> alg_dop_matrization(){
        matrix<T> mm(this->get_str(), this->get_stb());
        T* strok = new T[this->get_stb()];
        thread ttt([&](){
            for(int i = 0; i < this->get_str() / 2; i++){
                for(int j = 0; j < this->get_stb(); j++){
                    strok[j] = this->alg_dop(i + 1, j + 1);
                }
                mm.update_strok(i, strok);
            }
        });
        for(int i = this->get_str() / 2; i < this->get_str(); i++){
            for(int j = 0; j < this->get_stb(); j++){
                strok[j] = this->alg_dop(i + 1, j + 1);
            }
            mm.update_strok(i, strok);
        }
        ttt.join();
        delete [] strok;
        return mm;
    }
    void transposing(){
        thread tt([&](){
            for(int i = 0; i < this->get_str() / 2; i++){
            for(int j = i; j < this->get_stb(); j++){
                T c = this->el[i][j];
                el[i][j] = el[j][i];
                el[j][i] = c;
            }
        }
        });

        for(int i = this->get_str() / 2; i < this->get_str(); i++){
            for(int j = i; j < this->get_stb(); j++){
                T c = this->el[i][j];
                el[i][j] = el[j][i];
                el[j][i] = c;
            }
        }
        tt.join();
    }
  
  public:
    template <typename F> friend void diagonal_updating(matrix<F> &m, F c);
    template <typename F> friend void over_diagonal_updating(matrix<F> &m, F c);
    template <typename F> friend void under_diagonal_updating(matrix<F> &m, F c);

    template <typename F> friend void summarise1(matrix<F> &m0, matrix<F> &m1, const matrix<F> &m2);
    template <typename F> friend void summarise2(matrix<F> &m0, matrix<F> &m1, const matrix<F> &m2);

    template <typename F> friend void minusrise1(matrix<F> &m0, matrix<F> &m1, const matrix<F> &m2);
    template <typename F> friend void minusrise2(matrix<F> &m0, matrix<F> &m1, const matrix<F> &m2);

    template <typename F> friend void umnrise1(matrix<F> &m0, const F c);
    template <typename F> friend void umnrise2(matrix<F> &m0, const F c); 

    template <typename F> friend void delrise1(matrix<F> &m0, const F c);
    template <typename F> friend void delrise2(matrix<F> &m0, const F c);   

    template <typename F> friend bool checking_diagonal(matrix<F> &m0, const F c);
    template <typename F> friend bool checking_over_diagonal(matrix<F> &m0, F c);
    template <typename F> friend bool checking_under_diagonal(matrix<F> &m0, F c);

    template <typename F> friend bool checking_diagonal(matrix<F> &m0, const matrix<F> &mm);
    template <typename F> friend bool checking_under_diagonal(matrix<F> &m0, const matrix<F> &mm);
    template <typename F> friend bool checking_over_diagonal(matrix<F> &m0, const matrix<F> &mm);

    template <typename F> friend bool r_checking_diagonal(matrix<F> &m0, const matrix<F> &mm);
    template <typename F> friend bool r_checking_under_diagonal(matrix<F> &m0, const matrix<F> &mm);
    template <typename F> friend bool r_checking_over_diagonal(matrix<F> &m0, const matrix<F> &mm);
};

template <typename T>
void perestanovka(matrix<T> &m, int a, int b){
    if (a > m.get_str() || b > m.get_str() || a <= 0 || b <= 0){
        cout << "perestanovka: Error!" << endl;
    }
    else{
        a--;
        b--;
        T *stroka = new T[m.get_stb()];
        for(int i = 0; i < m.get_stb(); i++){
            stroka[i] = m.get_strok(a)[i];
        }
        m.update_strok(a, m.get_strok(b));
        m.update_strok(b, stroka);
        delete[] stroka;
    }
}
template <typename T>
void peremnozhenie(matrix<T> &m, int a, int b, int d){
    if (a > m.get_str() || b > m.get_str() || a <= 0 || b <= 0){
        cout << "peremnozhenie: Error!" << endl;
    }
    else{
        a--;
        b--;
        T *stroka = new T[m.get_stb()];
        for(int i = 0; i < m.get_stb(); i++){
            stroka[i] = m.get_strok(a)[i] * d + m.get_strok(b)[i];
        }
        m.update_strok(b, stroka);
        delete[] stroka;
    }
}
template <typename T>
void umnozhenie(matrix<T> &m, int a, int d){
    if (a > m.get_str() || a <= 0){
        cout << "umnozhenie: Error!" << endl;
    }
    else{
        a--;
        T *stroka = new T[m.get_stb()];
        for(int i = 0; i < m.get_stb(); i++){
            stroka[i] = m.get_strok(a)[i] * d;
        }
        m.update_strok(a, stroka);
        delete[] stroka;
    }
}

template <typename T>
void diagonal_updating(matrix<T> &m, T c){
    for(int i = 0; i < m.stb; i++){
        m.el[i][i] = c;
    }
}
template <typename T>
void over_diagonal_updating(matrix<T> &m, T c){
    for(int i = 0; i < m.str; i++){
        for(int j = i + 1; j < m.stb; j++){
            m.el[i][j] = c;
        }
    }
}
template <typename T>
void under_diagonal_updating(matrix<T> &m, T c){
    for(int i = 0; i < m.str; i++){
        for(int j = 0; j < i; j++){
            m.el[i][j] = c;
        }
    }
}

template <typename T>
void summarise1(matrix<T> &m0, matrix<T> &m1, const matrix<T> &m2){
    for(int i = 0 ; i < m0.str/2 + 1; i++){
        for(int j = 0; j < m0.stb; j++){
            m0.el[i][j] = m1.el[i][j] + m2.el[i][j];
        }
    }
}
template <typename T>
void summarise2(matrix<T> &m0, matrix<T>& m1, const matrix<T> &m2){
    for(int i = m0.str/2 + 1; i < m0.str; i++){
        for(int j = 0; j < m0.stb; j++){
            m0.el[i][j] = m1.el[i][j] + m2.el[i][j];
        }
    }
}

template <typename T>
void umnrise1(matrix<T> &m0, const T c){
    for(int i = 0 ; i < m0.str/2 + 1; i++){
        for(int j = 0; j < m0.stb; j++){
            m0.el[i][j] = m0.el[i][j] * c;
        }
    }
}
template <typename T>
void umnrise2(matrix<T> &m0, const T c){
    for(int i = m0.str/2 + 1; i < m0.str; i++){
        for(int j = 0; j < m0.stb; j++){
            m0.el[i][j] = m0.el[i][j] * c;
        }
    }
}

template <typename T>
void delrise1(matrix<T> &m0, const T c){
    for(int i = 0 ; i < m0.str/2 + 1; i++){
        for(int j = 0; j < m0.stb; j++){
            m0.el[i][j] = m0.el[i][j] / c;
        }
    }
}
template <typename T>
void delrise2(matrix<T> &m0, const T c){
    for(int i = m0.str/2 + 1; i < m0.str; i++){
        for(int j = 0; j < m0.stb; j++){
            m0.el[i][j] = m0.el[i][j] / c;
        }
    }
}

template <typename T>
void minusrise1(matrix<T> &m0, matrix<T> &m1, const matrix<T> &m2){
    for(int i = 0 ; i < m0.str/2 + 1; i++){
        for(int j = 0; j < m0.stb; j++){
            m0.el[i][j] = m1.el[i][j] - m2.el[i][j];
        }
    }
}
template <typename T>
void minusrise2(matrix<T> &m0, matrix<T>& m1, const matrix<T> &m2){
    for(int i = m0.str/2 + 1; i < m0.str; i++){
        for(int j = 0; j < m0.stb; j++){
            m0.el[i][j] = m1.el[i][j] - m2.el[i][j];
        }
    }
}

template <typename T>
bool checking_diagonal(matrix<T> &m0, const T c){
    for(int i = 0; i < m0.str; i++){
        if (m0.el[i][i] != c){
            return false;
        }
    }
    return true;
}
template <typename T>
bool checking_over_diagonal(matrix<T> &m0, T c){
    for(int i = 0; i < m0.str; i++){
        for(int j = i + 1; j < m0.stb; j++){
            if (m0.el[i][j] != c){
            return false;
            }
        }
    }
    return true;
}
template <typename T>
bool checking_under_diagonal(matrix<T> &m0, T c){
    for(int i = 0; i < m0.str; i++){
        for(int j = 0; j < i; j++){
            if (m0.el[i][j] != c){
            return false;
            }
        }
    }
    return true;
}

template <typename T>
bool checking_diagonal(matrix<T> &m0, const matrix<T> &mm){
    for(int i = 0; i < m0.str; i++){
        if (m0.el[i][i] != mm.el[i][i]){
            return false;
        }
    }
    return true;
}
template <typename T>
bool checking_under_diagonal(matrix<T> &m0, const matrix<T> &mm){
    for(int i = 0; i < m0.str; i++){
        for(int j = 0; j < i; j++){
            if(m0.el[i][j] != mm.el[i][j]){
                return false;
            }
        }
    }
    return true;
}
template <typename T>
bool checking_over_diagonal(matrix<T> &m0, const matrix<T> &mm){
    for(int i = 0; i < m0.str; i++){
        for(int j = i + 1; j < m0.stb; j++){
            if(m0.el[i][j] != mm.el[i][j]){
                return false;
            }
        }
    }
    return true;
}

template <typename T>
bool r_checking_diagonal(matrix<T> &m0, const matrix<T> &mm){
    for(int i = 0; i < m0.str; i++){
        if (m0.el[i][i] != mm.el[i][i]){
            return true;
        }
    }
    return false;
}
template <typename T>
bool r_checking_under_diagonal(matrix<T> &m0, const matrix<T> &mm){
    for(int i = 0; i < m0.str; i++){
        for(int j = 0; j < i; j++){
            if(m0.el[i][j] != mm.el[i][j]){
                return true;
            }
        }
    }
    return false;
}
template <typename T>
bool r_checking_over_diagonal(matrix<T> &m0, const matrix<T> &mm){
    for(int i = 0; i < m0.str; i++){
        for(int j = i + 1; j < m0.stb; j++){
            if(m0.el[i][j] != mm.el[i][j]){
                return true;
            }
        }
    }
    return false;
}


int main(){
    auto start = chrono::high_resolution_clock::now();
    matrix<int> m1;
    matrix<int> m2 = m1;
    m2.show_matrix();
    cout << (m2 == 0) << endl;
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<float> duration = end - start;
    cout << "duration " << duration.count() << endl;
    return 0;
}

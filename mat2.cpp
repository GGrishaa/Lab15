#include <iostream>
#include <fstream>
#include <thread>
#include <future>
#include <queue>
#include <mutex>

std::mutex mtx;
std::mutex mtxx;

using namespace std;

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
    matrix(int a = 0, int b = 0, T c = T(), int size = 1){
        str = a;
        stb = b;
        el = new T* [str];
        for(int i = 0; i < str; i++){
            el[i] = new T[stb];
        }

        int count = str / size;
        if (str % size != 0){
            count++;
        }
        //count--;
        queue<future<void>> q;
        for(int t = 0; t < count; t++){
            q.push(async([&](){
                for(int i = t * size; (i < size * t + size) && (i < str); i++){
                    for(int j = 0; j < stb; j++){
                        //mtxx.lock();
                        if (i == j){
                            el[i][j] = c;
                        }
                        else{
                            el[i][j] = T();
                        }
                        //mtxx.unlock();
                        mtx.lock();
                        cout << "(" << i << " " << j << ") have been filled by " << el[i][j] << " in block " << t + 1 << endl; 
                        mtx.unlock();
                    }
                }
            }));
        }
        while (!q.empty()){
            q.front().get();
            q.pop();
        }
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
        bool fl = true;
        if (this->str == other.str && this->stb == other.stb){
            fl = true;
            for(int i = 0; i < this->str; i++){
                if (fl){
                    for(int j = 0; j < this->stb; j++){
                        if (this->el[i][j] != other.el[i][j]){
                            fl = false;
                            break;
                        }
                    }
                }
            }
        } 
        else{
            fl = false;
        }
        return fl;
    }
    bool operator != (const matrix<T> &other){
        bool fl;
        if (this->str != other.str || this->stb != other.stb){
            fl = true;
        }
        else{
            fl = false;
            for(int i = 0; i < this->str; i++){
                if (!fl){
                    for(int j = 0; j < this->stb; j++){
                        if (this->el[i][j] != other.el[i][j]){
                            fl = true;
                        }
                    }
                }
            }
        }
        return fl;
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
        for(int i = 0; i < str; i++){
            for(int j = 0; j < stb; j++){
                this->el[i][j] = other.el[i][j];
            }
        }
        return *this;
    }
    bool operator == (const T a){
        bool fl;
        if (this->str != this->stb){
            fl = false;
        }
        else{
            fl = true;
            for(int i = 0; i < this->str; i++){
                if (fl){
                    for(int j = 0; j < this->stb; j++){
                        if (i == j){
                            if (this->el[i][j] != a){
                                fl = false;
                                break;
                            }
                        }
                        else{
                            if (this->el[i][j] != 0){
                                fl = false;
                                break;
                            }
                        }
                    }
                }
            }
        }
    return fl;
    }
    matrix<T> operator + (const matrix<T> &other){
        matrix<T> summa(this->str, this->stb);
        if(this->str != other.str || this->stb != other.stb){
            cout << "Error: \"+\"" << endl;
        }
        else{
            for(int i = 0; i < summa.str; i++){
                for(int j = 0; j < summa.stb; j++){
                    summa.el[i][j] = this->el[i][j] + other.el[i][j];
                }
            }
        }
        return summa;
    }
    matrix<T> operator - (const matrix<T> &other){
        matrix<T> razn(this->str, this->stb);
        if(this->str != other.str || this->stb != other.stb){
            cout << "Error: \"-\"" << endl;
        }
        else{
            for(int i = 0; i < razn.str; i++){
                for(int j = 0; j < razn.stb; j++){
                    razn.el[i][j] = this->el[i][j] - other.el[i][j];
                }
            }
        }
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
        matrix<T> um(this->str, this->stb);
        for(int i = 0; i < this->str; i++){
            for(int j = 0; j < this->stb; j++){
                um.el[i][j] = a * this->el[i][j];
            }
        }
        return um;
    }
    matrix<T> operator / (const T a){
        matrix<T> del(this->str, this->stb);
        for(int i = 0; i < this->str; i++){
            for(int j = 0; j < this->stb; j++){
                del.el[i][j] = this->el[i][j] / a;
            }
        }
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
            for(int i = 0; i < this->get_stb(); i++){
                d += pow((-1), i) * this->el[0][i] * ((this->matrix_without_strok(1)).matrix_without_stolb(i + 1)).determinant();
                //cout << "Без столбика " << i + 1 << " определитель = " << d << endl;
                //matrix<T> time_matrix = (this->matrix_without_strok(1)).matrix_without_stolb(i+1);
                //cout << "=========================" << endl << "матрица " << i + 1 << " :" << endl;
                //time_matrix.show_matrix();
                //cout << "ее определитель = " << time_matrix.determinant() << endl;
            }
            return d;
        }
        return T();
    }
    matrix<T> operator ! (){
        if(this->get_str() != this->get_stb()){
            cout << "Error, matrix must be squared" << endl;
            return *this;
        }
        T c = this->determinant();
        if (c == 0){
            matrix<T> m0(this->get_str(), this->get_stb());
            cout << "Error, matrix's determinant must be not 0" << endl;
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
        for(int i = 0; i < this->get_str(); i++){
            for(int j = 0; j < this->get_stb(); j++){
                strok[j] = this->alg_dop(i + 1, j + 1);
            }
            mm.update_strok(i, strok);
            }
        delete [] strok;
        return mm;
    }
    void transposing(){
        for(int i = 0; i < this->get_str(); i++){
            for(int j = i; j < this->get_stb(); j++){
                T c = this->el[i][j];
                el[i][j] = el[j][i];
                el[j][i] = c;
            }
        }
    }
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


int main(){
    matrix<int> m3(5, 5, 4, 2);
    m3.show_matrix();
    return 0;
}
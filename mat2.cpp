#include <iostream>
#include <fstream>
#include <thread>
#include <future>
#include <queue>

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
    int size;
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
        size = 1;
    }
    matrix(int a = 0, int b = 0, T c = T(), int new_size = 1){
        str = a;
        stb = b;
        this->size = new_size;
        el = new T* [str];
        for(int i = 0; i < str; i++){
            el[i] = new T[stb];
        }

        int count = str / size;
        if (str % size != 0){
            count++;
        }
        queue<future<void>> q;
        for(int t = 0; t < count; t++){
            q.push(async([&, t](){
                for(int i = t * size; (i < size * t + size) && (i < str); i++){
                    for(int j = 0; j < stb; j++){
                        if (i == j){
                            el[i][j] = c;
                        }
                        else{
                            el[i][j] = T();
                        }
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
        this->size = other.size;
        el = new T*[str];
        for(int i = 0; i < str; i++){
            el[i] = new T[stb];
        }

        int count = str / size;
        if (str % size != 0){
            count++;
        }
        queue<future<void>> q;
        for(int t = 0; t < count; t++){
            q.push(async([&, t](){
                for(int i = t * size; (i < size * t + size) && (i < str); i++){
                    for(int j = 0; j < stb; j++){
                        el[i][j] = other.el[i][j];
                    }
                }
            }));
        }
        while (!q.empty()){
            q.front().get();
            q.pop();
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
    int get_size(){
        return size;
    }
    void update_size(int new_size){
        if(new_size >= 1){
            this->size = new_size;
        }
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
            int count = str / size;
            if (str % size != 0){
                count++;
            }
            queue<future<void>> q;
            for(int t = 0; t < count; t++){
                q.push(async([&, t](){
                    for(int i = t * size; (i < size * t + size) && (i < str); i++){
                        for(int j = 0; j < stb; j++){
                            if(this->el[i][j] != other.el[i][j]){
                                fl = false;
                            }
                        }
                    }
                }));
            }
            while (!q.empty()){
                q.front().get();
                q.pop();
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
            int count = str / size;
            if (str % size != 0){
                count++;
            }
            queue<future<void>> q;
            for(int t = 0; t < count; t++){
                q.push(async([&, t](){
                    for(int i = t * size; (i < size * t + size) && (i < str); i++){
                        for(int j = 0; j < stb; j++){
                            if(this->el[i][j] != other.el[i][j]){
                                fl = true;
                            }
                        }
                    }
                }));
            }
            while (!q.empty()){
                q.front().get();
                q.pop();
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
        this->size = other.size;
        el = new T*[str];
        for(int i = 0; i < str; i++){
            el[i] = new T[stb];
        }
        int count = str / size;
        if (str % size != 0){
            count++;
        }
        queue<future<void>> q;
        for(int t = 0; t < count; t++){
            q.push(async([&, t](){
                for(int i = t * size; (i < size * t + size) && (i < str); i++){
                    for(int j = 0; j < stb; j++){
                        this->el[i][j] = other.el[i][j];
                    }
                }
            }));
        }
        while (!q.empty()){
            q.front().get();
            q.pop();
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
            int count = str / size;
            if (str % size != 0){
                count++;
            }
            queue<future<void>> q;
            for(int t = 0; t < count; t++){
                q.push(async([&, t](){
                    for(int i = t * size; (i < size * t + size) && (i < str); i++){
                        for(int j = 0; j < stb; j++){
                            if (i == j && this->el[i][j] != a){
                                fl = false;
                            }
                            else if (i != j && this->el[i][j] != T()){
                                fl = false;
                            }
                        }
                    }
                }));
            }
            while (!q.empty()){
                q.front().get();
                q.pop();
            }
        }
        return fl;
    }
    matrix<T> operator + (const matrix<T> &other){
        matrix<T> summa(this->str, this->stb);
        summa.update_size(size);
        if(this->str != other.str || this->stb != other.stb){
            cout << "Error: \"+\"" << endl;
        }
        else{
            int count = str / size;
            if (str % size != 0){
                count++;
            }
            queue<future<void>> q;
            for(int t = 0; t < count; t++){
                q.push(async([&, t](){
                    for(int i = t * size; (i < size * t + size) && (i < str); i++){
                        for(int j = 0; j < stb; j++){
                            summa.el[i][j] = this->el[i][j] + other.el[i][j];
                        }
                    }
                }));
            }
            while (!q.empty()){
                q.front().get();
                q.pop();
            }
        }
        return summa;
    }
    matrix<T> operator - (const matrix<T> &other){
        matrix<T> razn(this->str, this->stb);
        razn.update_size(size);
        if(this->str != other.str || this->stb != other.stb){
            cout << "Error: \"-\"" << endl;
        }
        else{
            int count = str / size;
            if (str % size != 0){
                count++;
            }
            queue<future<void>> q;
            for(int t = 0; t < count; t++){
                q.push(async([&, t](){
                    for(int i = t * size; (i < size * t + size) && (i < str); i++){
                        for(int j = 0; j < stb; j++){
                            razn.el[i][j] = this->el[i][j] - other.el[i][j];
                        }
                    }
                }));
            }
            while (!q.empty()){
                q.front().get();
                q.pop();
            }
        }
        return razn;
    }
    matrix<T> operator * (const matrix<T> &other){
        matrix<T> pro(this->str, other.stb);
        pro.update_size(size);
        int a = this->stb;
        if (this->stb != other.str){
            cout << "Error: \"*\"" << endl;
        }
        else{
            int count = pro.str / size;
            if (pro.str % size != 0){
                count++;
            }
            queue<future<void>> q;
            for(int t = 0; t < count; t++){
                q.push(async([&, t](){
                    for(int i = t * size; (i < size * t + size) && (i < pro.str); i++){
                        for(int j = 0; j < pro.stb; j++){
                            for(int p = 0; p < a; p++){
                                pro.el[i][j] += this->el[i][p] * other.el[p][j];
                            }
                        }
                    }
                }));
            }
            while (!q.empty()){
                q.front().get();
                q.pop();
            }
        }
        return pro;
    }
    matrix<T> operator * (const T a){
        matrix<T> um(this->str, this->stb);
        um.update_size(size);
        int count = str / size;
        if (str % size != 0){
            count++;
        }
        queue<future<void>> q;
        for(int t = 0; t < count; t++){
            q.push(async([&, t](){
                for(int i = t * size; (i < size * t + size) && (i < str); i++){
                    for(int j = 0; j < stb; j++){
                        um.el[i][j] = this->el[i][j] * a;
                    }
                }
            }));
        }
        while (!q.empty()){
            q.front().get();
            q.pop();
        }
        return um;
    }
    matrix<T> operator / (const T a){
        matrix<T> del(this->str, this->stb);
        del.update_size(size);
        int count = str / size;
        if (str % size != 0){
            count++;
        }
        queue<future<void>> q;
        for(int t = 0; t < count; t++){
            q.push(async([&, t](){
                for(int i = t * size; (i < size * t + size) && (i < str); i++){
                    for(int j = 0; j < stb; j++){
                        del.el[i][j] = this->el[i][j] / a;
                    }
                }
            }));
        }
        while (!q.empty()){
            q.front().get();
            q.pop();
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
        if(this->stb != this->str){
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
            int count = stb / size;
            if (stb % size != 0){
                count++;
            }
            queue<future<void>> q;
            for(int t = 0; t < count; t++){
                q.push(async([&, t](){
                    for(int j = size * t; (j < (t + 1) * size) && j < stb; j++){
                        d += pow((-1), j) * this->el[0][j] * ((this->matrix_without_strok(1)).matrix_without_stolb(j + 1)).determinant();
                    }
                }));
            }
            while (!q.empty()){
                q.front().get();
                q.pop();
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
        int count = str / size;
        if (str % size != 0){
            count++;
        }
        queue<future<void>> q;
        for(int t = 0; t < count; t++){
            q.push(async([&, t](){
                for(int i = t * size; (i < (t + 1) * size) && (i < str); i++){
                    for(int j = number; j < stb - 1; j++){
                        this->el[i][j] = this->el[i][j + 1];
                    }
                }
            }));
        }
        while (!q.empty()){
            q.front().get();
            q.pop();
        }
        this->stb = this->get_stb() - 1;
    }
    T alg_dop(int a, int b){
        return (pow((-1), a + b) * (this->matrix_without_strok(a).matrix_without_stolb(b)).determinant());
    }
    matrix<T> alg_dop_matrization(){
        if(str != stb){
            return *this;
        }
        matrix<T> mm(this->get_str(), this->get_stb());
        int count = str / size;
        if (str % size != 0){
            count++;
        }
        queue<future<void>> q;
        for(int t = 0; t < count; t++){
            q.push(async([&, t](){
                T* strok = new T[stb];
                for(int i = t * size; (i < str) && (i < (t + 1) * size); i++){
                    for(int j = 0; j < stb; j++){
                        strok[j] = this->alg_dop(i + 1, j + 1);
                    }
                    mm.update_strok(i, strok);
                }
                delete[] strok;
            }));
        }
        while (!q.empty()){
            q.front().get();
            q.pop();
        }
        return mm;
    }
    void transposing(){
        int count = str / size;
        if (str % size != 0){
            count++;
        }
        queue<future<void>> q;
        for(int t = 0; t < count; t++){
            q.push(async([&, t](){
                for(int i = t * size; (i < size * t + size) && (i < str); i++){
                    for(int j = i; j < stb; j++){
                        T c = el[i][j];
                        el[i][j] = el[j][i];
                        el[j][i] = c;
                    }
                }
            }));
        }
        while (!q.empty()){
            q.front().get();
            q.pop();
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
    matrix<int> m = matrix<int>::single_matrix(3);
    m.update_size(2);
    m.show_matrix();
    matrix<int> m2 = m.matrix_without_stolb(2);
    m2.show_matrix();
    return 0;
}
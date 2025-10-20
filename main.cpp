#include <iostream>

template <typename T>
class MatrizBase {
protected:
    int _filas;
    int _columnas;

public:
    MatrizBase(int filas, int columnas) : _filas(filas), _columnas(columnas) {}
    virtual ~MatrizBase() {}

    virtual void cargarValores() = 0;
    virtual MatrizBase<T>* sumar(const MatrizBase<T>& otra) const = 0;
    virtual void imprimir() const = 0;

    int getFilas() const { return _filas; }
    int getColumnas() const { return _columnas; }

    virtual T& at(int fila, int col) = 0;
    virtual const T& at(int fila, int col) const = 0;
};

template <typename T>
class MatrizDinamica : public MatrizBase<T> {
private:
    T **_datos;

public:
    MatrizDinamica(int filas, int columnas) : MatrizBase<T>(filas, columnas) {
        _datos = new T*[this->_filas];
        for (int i = 0; i < this->_filas; ++i) {
            _datos[i] = new T[this->_columnas];
        }
    }

    ~MatrizDinamica() override {
        std::cout << "Liberando memoria de Matriz Dinámica..." << std::endl;
        for (int i = 0; i < this->_filas; ++i) {
            delete[] _datos[i];
        }
        delete[] _datos;
    }

    MatrizDinamica(const MatrizDinamica& other) : MatrizBase<T>(other._filas, other._columnas) {
        _datos = new T*[this->_filas];
        for (int i = 0; i < this->_filas; ++i) {
            _datos[i] = new T[this->_columnas];
            for (int j = 0; j < this->_columnas; ++j) {
                _datos[i][j] = other._datos[i][j];
            }
        }
    }

    MatrizDinamica& operator=(const MatrizDinamica& other) {
        if (this == &other) return *this;

        for (int i = 0; i < this->_filas; ++i) {
            delete[] _datos[i];
        }
        delete[] _datos;

        this->_filas = other._filas;
        this->_columnas = other._columnas;
        _datos = new T*[this->_filas];
        for (int i = 0; i < this->_filas; ++i) {
            _datos[i] = new T[this->_columnas];
            for (int j = 0; j < this->_columnas; ++j) {
                _datos[i][j] = other._datos[i][j];
            }
        }
        return *this;
    }

    MatrizDinamica(MatrizDinamica&& other) noexcept : MatrizBase<T>(other._filas, other._columnas), _datos(other._datos) {
        other._filas = 0;
        other._columnas = 0;
        other._datos = nullptr;
    }

    MatrizDinamica& operator=(MatrizDinamica&& other) noexcept {
        if (this == &other) return *this;

        for (int i = 0; i < this->_filas; ++i) {
            delete[] _datos[i];
        }
        delete[] _datos;

        this->_filas = other._filas;
        this->_columnas = other._columnas;
        _datos = other._datos;

        other._filas = 0;
        other._columnas = 0;
        other._datos = nullptr;
        
        return *this;
    }

    T& at(int fila, int col) override {
        return _datos[fila][col];
    }

    const T& at(int fila, int col) const override {
        return _datos[fila][col];
    }

    void cargarValores() override {
        for (int i = 0; i < this->_filas; ++i) {
            for (int j = 0; j < this->_columnas; ++j) {
                _datos[i][j] = static_cast<T>((i + j) * (i + 1.5));
            }
        }
    }

    MatrizBase<T>* sumar(const MatrizBase<T>& otra) const override {
        if (this->_filas != otra.getFilas() || this->_columnas != otra.getColumnas()) {
            return nullptr;
        }
        MatrizDinamica<T>* resultado = new MatrizDinamica<T>(this->_filas, this->_columnas);
        for (int i = 0; i < this->_filas; ++i) {
            for (int j = 0; j < this->_columnas; ++j) {
                resultado->_datos[i][j] = this->_datos[i][j] + otra.at(i, j);
            }
        }
        return resultado;
    }

    void imprimir() const override {
        for (int i = 0; i < this->_filas; ++i) {
            std::cout << "| ";
            for (int j = 0; j < this->_columnas; ++j) {
                std::cout << _datos[i][j] << " | ";
            }
            std::cout << std::endl;
        }
    }
};

template <typename T, int M, int N>
class MatrizEstatica : public MatrizBase<T> {
private:
    T _datos[M][N];

public:
    MatrizEstatica() : MatrizBase<T>(M, N) {}
    
    T& at(int fila, int col) override {
        return _datos[fila][col];
    }

    const T& at(int fila, int col) const override {
        return _datos[fila][col];
    }

    void cargarValores() override {
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                _datos[i][j] = static_cast<T>((i + j + 1) * (j + 0.5));
            }
        }
    }

    MatrizBase<T>* sumar(const MatrizBase<T>& otra) const override {
        if (this->_filas != otra.getFilas() || this->_columnas != otra.getColumnas()) {
            return nullptr;
        }
        MatrizEstatica<T, M, N>* resultado = new MatrizEstatica<T, M, N>();
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < N; ++j) {
                resultado->_datos[i][j] = this->_datos[i][j] + otra.at(i, j);
            }
        }
        return resultado;
    }

    void imprimir() const override {
        for (int i = 0; i < M; ++i) {
            std::cout << "| ";
            for (int j = 0; j < N; ++j) {
                std::cout << _datos[i][j] << " | ";
            }
            std::cout << std::endl;
        }
    }
};

template <typename T>
MatrizBase<T>* operator+(const MatrizBase<T>& a, const MatrizBase<T>& b) {
    return a.sumar(b);
}

int main() {
    std::cout << "--- Sistema generico de Algebra Lineal ---" << std::endl;
    std::cout << ">> Demostracion de Genericidad (Tipo FLOAT) <<" << std::endl << std::endl;

    MatrizBase<float>* A = new MatrizDinamica<float>(3, 2);
    std::cout << "Creando Matriz Dinamica A (3x2)..." << std::endl;
    A->at(0,0) = 1.5; A->at(0,1) = 2.0;
    A->at(1,0) = 0.0; A->at(1,1) = 1.0;
    A->at(2,0) = 4.5; A->at(2,1) = 3.0;
    std::cout << "A =" << std::endl;
    A->imprimir();
    std::cout << std::endl;

    MatrizBase<float>* B = new MatrizEstatica<float, 3, 2>();
    std::cout << "Creando Matriz Estatica B (3x2)..." << std::endl;
    B->at(0,0) = 0.5; B->at(0,1) = 1.0;
    B->at(1,0) = 2.0; B->at(1,1) = 3.0;
    B->at(2,0) = 1.0; B->at(2,1) = 1.0;
    std::cout << "B =" << std::endl;
    B->imprimir();
    std::cout << std::endl;
    
    std::cout << "SUMANDO: Matriz C = A + B ..." << std::endl;
    MatrizBase<float>* C = *A + *B;
    
    std::cout << std::endl;
    std::cout << "Matriz Resultado C (3x2, Tipo FLOAT):" << std::endl;
    C->imprimir();
    std::cout << std::endl;

    std::cout << ">> Demostracion de Genericidad (Tipo INT) <<" << std::endl << std::endl;

    MatrizBase<int>* D = new MatrizDinamica<int>(2, 2);
    std::cout << "Creando Matriz Dinamica D (2x2)..." << std::endl;
    D->at(0,0) = 1; D->at(0,1) = 2;
    D->at(1,0) = 3; D->at(1,1) = 4;
    std::cout << "D =" << std::endl;
    D->imprimir();
    std::cout << std::endl;

    MatrizBase<int>* E = new MatrizEstatica<int, 2, 2>();
    std::cout << "Creando Matriz Estatica E (2x2)..." << std::endl;
    E->at(0,0) = 5; E->at(0,1) = 6;
    E->at(1,0) = 7; E->at(1,1) = 8;
    std::cout << "E =" << std::endl;
    E->imprimir();
    std::cout << std::endl;

    std::cout << "SUMANDO: Matriz F = D + E ..." << std::endl;
    MatrizBase<int>* F = *D + *E;

    std::cout << std::endl;
    std::cout << "Matriz Resultado F (2x2, Tipo INT):" << std::endl;
    F->imprimir();
    std::cout << std::endl;

    std::cout << ">> Demostracion de Limpieza de Memoria <<" << std::endl;
    std::cout << "Llamando al destructor de C..." << std::endl;
    delete C;
    std::cout << "Llamando al destructor de A..." << std::endl;
    delete A;
    std::cout << "Llamando al destructor de B..." << std::endl;
    delete B;
    std::cout << "Llamando al destructor de F..." << std::endl;
    delete F;
    std::cout << "Llamando al destructor de D..." << std::endl;
    delete D;
    std::cout << "Llamando al destructor de E..." << std::endl;
    delete E;
    
    std::cout << "Sistema cerrado." << std::endl;
    
    return 0;
}
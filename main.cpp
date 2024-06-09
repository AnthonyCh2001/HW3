//Integrantes:
//Chavez Ayme, Anthony
//Alferez Vicente, Bladimir
//Valdivia Schreiber, Mauricio Guillermo Manuel
#include <iostream>
#include <vector>
#include <list>
using namespace std;

struct data_unit {
    int data;
    int stamp;
    data_unit* next = nullptr;
};

class Tabla_Hash {
private:
    int size;
    int n_elements = 0;
    float maxfillfactor = 0.8;
    vector<list<data_unit*>> tabla;

public:
    Tabla_Hash(int sz) {
        size = sz;
        tabla.resize(size);
    }

    void insert(string key, int value) {
        data_unit* val = new data_unit;
        val->data = value;
        val->stamp = funcion_hash(key);
        if (tabla[val->stamp % size].empty()) {
            tabla[val->stamp % size].push_back(val);
        } else {
            data_unit* aux = tabla[val->stamp % size].front();
            while (aux->next != nullptr) {
                aux = aux->next;
            }
            aux->next = val;
        }
        n_elements++;
        if (need_rehash()) {
            rehashing();
        }
    }

    bool need_rehash() {
        float aux = static_cast<float>(n_elements) / size;
        return (aux > maxfillfactor);
    }

    void ajustar_maxfillfactor(float value) {
        maxfillfactor = value;
    }

    void rehashing() {
        int nuevoSize = size * 2;
        vector<list<data_unit*>> tabla_rehash(nuevoSize);

        for (auto& lista : tabla) {
            for (auto nodo : lista) {
                tabla_rehash[nodo->stamp % nuevoSize].push_back(nodo);
                while (nodo->next != nullptr) {
                    nodo = nodo->next;
                    tabla_rehash[nodo->stamp % nuevoSize].push_back(nodo);
                }
            }
        }
        tabla = tabla_rehash;
        size = nuevoSize;
    }

    int funcion_hash(string key) {
        int aux = 71;
        for (char letter : key) {
            aux = (aux << 3) * aux + letter * aux;
        }
        if(aux < 0){
            aux = aux * -1;
        }
        return aux;
    }

    data_unit* buscar(string key) {
        int hash = funcion_hash(key) % size;
        for (auto nodo : tabla[hash]) {
            data_unit* current = nodo;
            while (current != nullptr) {
                if (current->stamp == funcion_hash(key)) {
                    return current;
                }
                current = current->next;
            }
        }
        return nullptr;
    }

    bool eliminar(string key) {
        int hash = funcion_hash(key) % size;
        auto& lista = tabla[hash];
        auto it = lista.begin();
        data_unit* prev = nullptr;

        while (it != lista.end()) {
            data_unit* current = *it;
            if (current->stamp == funcion_hash(key)) {
                if (prev == nullptr) {
                    lista.erase(it);
                } else {
                    prev->next = current->next;
                    lista.erase(it);
                }
                delete current;
                n_elements--;
                return true;
            }
            prev = current;
            it++;
        }
        return false; 
    }
};

int main() {
    Tabla_Hash example(10);
    example.insert("hola", 1);
    example.insert("adios", 2);
    example.insert("aeaa", 3);
    example.insert("ded", 4);
    example.insert("eee", 5);
    example.insert("cccc", 6);
    example.insert("zxc", 7);
    example.insert("vfed", 8);
    example.insert("daxz", 9);
    example.insert("iki", 10);

    data_unit* encontrado = example.buscar("hola");
    if (encontrado != nullptr) {
        cout << "Elemento encontrado: " << encontrado->data << endl;
    } else {
        cout << "Elemento no encontrado" << endl;
    }

    if (example.eliminar("hola")) {
        cout << "Elemento eliminado exitosamente" << endl;
    } else {
        cout << "Elemento no encontrado para eliminar" << endl;
    }

    return 0;
}
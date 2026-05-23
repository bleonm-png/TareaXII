#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>

using namespace std;

struct Producto {
    int codigo;
    char nombre[50];
    float precio;
    int stock;
    bool activo;
};

const char* ARCHIVO = "productos.dat";
const float IVA = 0.13f;

void menuPrincipal();
void menuGestionProductos();
void registrarProducto();
void listarProductos();
void buscarProductoCodigo();
void buscarProductoNombre();
void modificarPrecio();
void actualizarStock();
void eliminarProducto();
void processoVenta();
bool existeProducto(int codigo);
Producto buscarProductoPorCodigo(int codigo);

int main() {
    menuPrincipal();
    return 0;
}

void menuPrincipal() {
    int opcion;

    do {
        cout << "\n===== SISTEMA DE GESTION DE PRODUCTOS =====\n";
        cout << "1. Gestion de productos\n";
        cout << "2. Proceso de venta\n";
        cout << "3. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                menuGestionProductos();
                break;
            case 2:
                processoVenta();
                break;
            case 3:
                cout << "Saliendo del sistema...\n";
                break;
            default:
                cout << "Opcion invalida.\n";
        }

    } while (opcion != 3);
}

void menuGestionProductos() {
    int opcion;

    do {
        cout << "\n===== GESTION DE PRODUCTOS =====\n";
        cout << "1. Registrar producto\n";
        cout << "2. Listar productos\n";
        cout << "3. Buscar por codigo\n";
        cout << "4. Buscar por nombre\n";
        cout << "5. Modificar precio\n";
        cout << "6. Actualizar stock\n";
        cout << "7. Eliminar producto\n";
        cout << "8. Volver\n";
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                registrarProducto();
                break;
            case 2:
                listarProductos();
                break;
            case 3:
                buscarProductoCodigo();
                break;
            case 4:
                buscarProductoNombre();
                break;
            case 5:
                modificarPrecio();
                break;
            case 6:
                actualizarStock();
                break;
            case 7:
                eliminarProducto();
                break;
            case 8:
                break;
            default:
                cout << "Opcion invalida.\n";
        }

    } while (opcion != 8);
}

void registrarProducto() {
    Producto p;

    cout << "\n--- Registrar producto ---\n";
    cout << "Codigo: ";
    cin >> p.codigo;

    if (existeProducto(p.codigo)) {
        cout << "Ya existe un producto con ese codigo.\n";
        return;
    }

    cin.ignore();

    cout << "Nombre: ";
    cin.getline(p.nombre, 50);

    cout << "Precio: ";
    cin >> p.precio;

    if (p.precio < 0) {
        cout << "El precio no puede ser negativo.\n";
        return;
    }

    cout << "Stock: ";
    cin >> p.stock;

    if (p.stock < 0) {
        cout << "El stock no puede ser negativo.\n";
        return;
    }

    p.activo = true;

    ofstream archivo(ARCHIVO, ios::binary | ios::app);

    if (!archivo) {
        cout << "Error al abrir el archivo.\n";
        return;
    }

    archivo.write(reinterpret_cast<char*>(&p), sizeof(Producto));
    archivo.close();

    cout << "Producto registrado correctamente.\n";
}

void listarProductos() {
    Producto p;

    ifstream archivo(ARCHIVO, ios::binary);

    if (!archivo) {
        cout << "No hay productos registrados.\n";
        return;
    }

    cout << "\n--- Lista de productos activos ---\n";
    cout << left << setw(10) << "Codigo"
         << setw(25) << "Nombre"
         << setw(12) << "Precio"
         << setw(10) << "Stock" << endl;

    cout << "----------------------------------------------------\n";

    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(Producto))) {
        if (p.activo) {
            cout << left << setw(10) << p.codigo
                 << setw(25) << p.nombre
                 << setw(12) << fixed << setprecision(2) << p.precio
                 << setw(10) << p.stock << endl;
        }
    }

    archivo.close();
}

void buscarProductoCodigo() {
    Producto p;
    int codigoBuscado;
    bool encontrado = false;

    cout << "\n--- Buscar producto por codigo ---\n";
    cout << "Ingrese codigo: ";
    cin >> codigoBuscado;

    ifstream archivo(ARCHIVO, ios::binary);

    if (!archivo) {
        cout << "No hay productos registrados.\n";
        return;
    }

    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(Producto))) {
        if (p.codigo == codigoBuscado && p.activo) {
            cout << "\nProducto encontrado:\n";
            cout << "Codigo: " << p.codigo << endl;
            cout << "Nombre: " << p.nombre << endl;
            cout << "Precio: Q" << fixed << setprecision(2) << p.precio << endl;
            cout << "Stock: " << p.stock << endl;

            encontrado = true;
            break;
        }
    }

    archivo.close();

    if (!encontrado) {
        cout << "Producto no encontrado.\n";
    }
}

void buscarProductoNombre() {
    Producto p;
    char nombreBuscado[50];
    bool encontrado = false;

    cout << "\n--- Buscar producto por nombre ---\n";
    cin.ignore();
    cout << "Ingrese nombre: ";
    cin.getline(nombreBuscado, 50);

    ifstream archivo(ARCHIVO, ios::binary);

    if (!archivo) {
        cout << "No hay productos registrados.\n";
        return;
    }

    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(Producto))) {
        if (strstr(p.nombre, nombreBuscado) != nullptr && p.activo) {
            cout << "\nProducto encontrado:\n";
            cout << "Codigo: " << p.codigo << endl;
            cout << "Nombre: " << p.nombre << endl;
            cout << "Precio: Q" << fixed << setprecision(2) << p.precio << endl;
            cout << "Stock: " << p.stock << endl;

            encontrado = true;
        }
    }

    archivo.close();

    if (!encontrado) {
        cout << "Producto no encontrado.\n";
    }
}

void modificarPrecio() {
    Producto p;
    int codigoBuscado;
    bool encontrado = false;
    float nuevoPrecio;

    cout << "\n--- Modificar precio ---\n";
    cout << "Ingrese codigo del producto: ";
    cin >> codigoBuscado;

    fstream archivo(ARCHIVO, ios::binary | ios::in | ios::out);

    if (!archivo) {
        cout << "No hay productos registrados.\n";
        return;
    }

    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(Producto))) {
        if (p.codigo == codigoBuscado && p.activo) {
            cout << "Precio actual: Q" << fixed << setprecision(2) << p.precio << endl;
            cout << "Nuevo precio: Q";
            cin >> nuevoPrecio;

            if (nuevoPrecio < 0) {
                cout << "El precio no puede ser negativo.\n";
                return;
            }

            p.precio = nuevoPrecio;

            archivo.seekp(-static_cast<int>(sizeof(Producto)), ios::cur);
            archivo.write(reinterpret_cast<char*>(&p), sizeof(Producto));

            encontrado = true;
            cout << "Precio actualizado correctamente.\n";
            break;
        }
    }

    archivo.close();

    if (!encontrado) {
        cout << "Producto no encontrado.\n";
    }
}

void actualizarStock() {
    Producto p;
    int codigoBuscado;
    bool encontrado = false;
    int nuevoStock;

    cout << "\n--- Actualizar stock ---\n";
    cout << "Ingrese codigo del producto: ";
    cin >> codigoBuscado;

    fstream archivo(ARCHIVO, ios::binary | ios::in | ios::out);

    if (!archivo) {
        cout << "No hay productos registrados.\n";
        return;
    }

    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(Producto))) {
        if (p.codigo == codigoBuscado && p.activo) {
            cout << "Stock actual: " << p.stock << endl;
            cout << "Nuevo stock: ";
            cin >> nuevoStock;

            if (nuevoStock < 0) {
                cout << "El stock no puede ser negativo.\n";
                return;
            }

            p.stock = nuevoStock;

            archivo.seekp(-static_cast<int>(sizeof(Producto)), ios::cur);
            archivo.write(reinterpret_cast<char*>(&p), sizeof(Producto));

            encontrado = true;
            cout << "Stock actualizado correctamente.\n";
            break;
        }
    }

    archivo.close();

    if (!encontrado) {
        cout << "Producto no encontrado.\n";
    }
}

void eliminarProducto() {
    Producto p;
    int codigoBuscado;
    bool encontrado = false;

    cout << "\n--- Eliminar producto ---\n";
    cout << "Ingrese codigo del producto: ";
    cin >> codigoBuscado;

    fstream archivo(ARCHIVO, ios::binary | ios::in | ios::out);

    if (!archivo) {
        cout << "No hay productos registrados.\n";
        return;
    }

    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(Producto))) {
        if (p.codigo == codigoBuscado && p.activo) {
            p.activo = false;

            archivo.seekp(-static_cast<int>(sizeof(Producto)), ios::cur);
            archivo.write(reinterpret_cast<char*>(&p), sizeof(Producto));

            encontrado = true;
            cout << "Producto eliminado correctamente.\n";
            break;
        }
    }

    archivo.close();

    if (!encontrado) {
        cout << "Producto no encontrado.\n";
    }
}

bool existeProducto(int codigo) {
    Producto p;

    ifstream archivo(ARCHIVO, ios::binary);

    if (!archivo) {
        return false;
    }

    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(Producto))) {
        if (p.codigo == codigo && p.activo) {
            archivo.close();
            return true;
        }
    }

    archivo.close();
    return false;
}

Producto buscarProductoPorCodigo(int codigo) {
    Producto p;
    p.codigo = -1;

    ifstream archivo(ARCHIVO, ios::binary);

    if (!archivo) {
        return p;
    }

    while (archivo.read(reinterpret_cast<char*>(&p), sizeof(Producto))) {
        if (p.codigo == codigo && p.activo) {
            archivo.close();
            return p;
        }
    }

    archivo.close();
    p.codigo = -1;
    return p;
}

void processoVenta() {
    Producto p;
    int codigoProducto;
    int cantidad;
    float subtotal, iva, total;

    cout << "\n===== PROCESO DE VENTA =====\n";
    cout << "Ingrese codigo del producto: ";
    cin >> codigoProducto;

    p = buscarProductoPorCodigo(codigoProducto);

    if (p.codigo == -1) {
        cout << "Producto no encontrado.\n";
        return;
    }

    cout << "\n--- Informacion del producto ---\n";
    cout << "Nombre: " << p.nombre << endl;
    cout << "Precio unitario: Q" << fixed << setprecision(2) << p.precio << endl;
    cout << "Stock disponible: " << p.stock << endl;

    cout << "\nIngrese cantidad a vender: ";
    cin >> cantidad;

    if (cantidad <= 0) {
        cout << "La cantidad debe ser mayor a cero.\n";
        return;
    }

    if (cantidad > p.stock) {
        cout << "Stock insuficiente. Maximo disponible: " << p.stock << endl;
        return;
    }

    subtotal = p.precio * cantidad;
    iva = subtotal * IVA;
    total = subtotal + iva;

    cout << "\n--- Resumen de venta ---\n";
    cout << "Producto: " << p.nombre << endl;
    cout << "Cantidad: " << cantidad << endl;
    cout << "Precio unitario: Q" << fixed << setprecision(2) << p.precio << endl;
    cout << "Subtotal: Q" << subtotal << endl;
    cout << "IVA (13%): Q" << iva << endl;
    cout << "TOTAL: Q" << total << endl;

    char confirmar;
    cout << "\nConfirmar venta (s/n): ";
    cin >> confirmar;

    if (confirmar == 's' || confirmar == 'S') {
        p.stock -= cantidad;

        fstream archivo(ARCHIVO, ios::binary | ios::in | ios::out);

        if (!archivo) {
            cout << "Error al actualizar stock.\n";
            return;
        }

        Producto temp;
        while (archivo.read(reinterpret_cast<char*>(&temp), sizeof(Producto))) {
            if (temp.codigo == codigoProducto) {
                archivo.seekp(-static_cast<int>(sizeof(Producto)), ios::cur);
                archivo.write(reinterpret_cast<char*>(&p), sizeof(Producto));
                break;
            }
        }

        archivo.close();

        cout << "Venta realizada correctamente.\n";
    } else {
        cout << "Venta cancelada.\n";
    }
}

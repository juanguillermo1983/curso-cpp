#include <iostream>
#include <string>

// Define una función para configurar el puerto serie
void configurarPuertoSerie() {
    // Coloca aquí el código para la configuración del puerto serie
    std::cout << "Configuracion del puerto serie..." << std::endl;
}

// Define una función para ejecutar el test del puerto serie
void ejecutarTestPuertoSerie() {
    // Coloca aquí el código para ejecutar el test del puerto serie
    std::cout << "Ejecutando test del puerto serie..." << std::endl;
}

int main() {
    int opcion;

    // Menú de opciones
    do {
        std::cout << "Menu:" << std::endl;
        std::cout << "1. Configuracion 1" << std::endl;
        std::cout << "2. Configuracion 2" << std::endl;
        std::cout << "0. Salir Default" << std::endl;
        std::cout << "Ingrese su opcion: ";
        std::cin >> opcion;

        switch (opcion) {
        case 1:
            configurarPuertoSerie();
            /*
            
                definir la configuracion 
            */

            break;
        case 2:
            ejecutarTestPuertoSerie();

            /*

                definir la configuracion
            */
            break;
        case 0:
            std::cout << "Saliendo..." << std::endl;

            /*

                definir la configuracion  conf default 
            */
            break;
        default:
            std::cout << "Opción no válida. Intente de nuevo." << std::endl;
        }
    } while (opcion != 0);







    return 0;
}
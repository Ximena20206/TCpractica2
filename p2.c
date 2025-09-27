#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int numLenguajes = 3;
// Nodo de lista doble
typedef struct Nodo {
    char *palabra;
    struct Nodo *anterior;
    struct Nodo *siguiente;
} Nodo;

// Lista doble
typedef struct {
    Nodo *cabeza;
    Nodo *cola;
} ListaDoble;

// ======== Funciones de lista ========

void inicializarLista(ListaDoble *l) {
    l->cabeza = NULL;
    l->cola = NULL;
}

int compararPalabras(const char *a, const char *b) {
    int lenA = strlen(a), lenB = strlen(b);
    if (lenA != lenB) return lenA - lenB;
    return strcmp(a, b);
}

int existePalabra(ListaDoble *l, const char *palabra) {
    Nodo *actual = l->cabeza;
    while (actual) {
        if (strcmp(actual->palabra, palabra) == 0) return 1;
        actual = actual->siguiente;
    }
    return 0;
}

void insertarOrdenado(ListaDoble *l, const char *palabra) {
    if (existePalabra(l, palabra)) return;

    Nodo *nuevo = (Nodo *)malloc(sizeof(Nodo));
    nuevo->palabra = strdup(palabra);
    nuevo->anterior = nuevo->siguiente = NULL;

    if (l->cabeza == NULL) {
        l->cabeza = l->cola = nuevo;
        return;
    }

    Nodo *actual = l->cabeza;
    while (actual && compararPalabras(actual->palabra, palabra) < 0) {
        actual = actual->siguiente;
    }

    if (actual == l->cabeza) {
        nuevo->siguiente = l->cabeza;
        l->cabeza->anterior = nuevo;
        l->cabeza = nuevo;
    } else if (actual == NULL) {
        nuevo->anterior = l->cola;
        l->cola->siguiente = nuevo;
        l->cola = nuevo;
    } else {
        nuevo->siguiente = actual;
        nuevo->anterior = actual->anterior;
        actual->anterior->siguiente = nuevo;
        actual->anterior = nuevo;
    }
}

void mostrarLista(ListaDoble *l) {
    Nodo *actual = l->cabeza;
    while (actual) {
        printf("%s\n", actual->palabra);
        actual = actual->siguiente;
    }
}

void liberarLista(ListaDoble *l) {
    Nodo *actual = l->cabeza;
    while (actual) {
        Nodo *tmp = actual;
        actual = actual->siguiente;
        free(tmp->palabra);
        free(tmp);
    }
    l->cabeza = l->cola = NULL;
}

void agregarLenguaje(ListaDoble ***LENGUAJES, int *numLenguajes, ListaDoble *lenguaje) {
    // Redimensionar el arreglo para un lenguaje más
    ListaDoble **tmp = (ListaDoble **)realloc(*LENGUAJES, (*numLenguajes + 1) * sizeof(ListaDoble *));
    if (!tmp) {
        printf("Error: no se pudo redimensionar LENGUAJES\n");
        return;
    }
    *LENGUAJES = tmp;

    // Insertar el puntero al lenguaje ya creado
    (*LENGUAJES)[*numLenguajes] = lenguaje;

    // Incrementar el contador
    (*numLenguajes)++;
}

// ======== Función para cargar un archivo en una lista ========

void cargarArchivoEnLista(const char *nombreArchivo, ListaDoble *lista) {
    FILE *f = fopen(nombreArchivo, "r");
    if (!f) {
        perror("Error al abrir archivo");
        return;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), f)) {
        buffer[strcspn(buffer, "\n")] = '\0'; // quitar salto de línea
        if (strlen(buffer) > 0) {
            insertarOrdenado(lista, buffer);
        }
    }

    fclose(f);
}

// ======== Programa principal ========

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Uso: %s <archivo1> <archivo2> <archivo3>\n", argv[0]);
        return 1;
    }

    

    // Arreglo de punteros a listas
    ListaDoble **LENGUAJES = (ListaDoble **)malloc(numLenguajes * sizeof(ListaDoble *));
    if (!LENGUAJES) {
        printf("Error: no se pudo reservar memoria para LENGUAJES\n");
        return 1;
    }

    // Inicializar y cargar cada lenguaje
    for (int i = 0; i < numLenguajes; i++) {
        LENGUAJES[i] = (ListaDoble *)malloc(sizeof(ListaDoble));
        inicializarLista(LENGUAJES[i]);
        cargarArchivoEnLista(argv[i + 1], LENGUAJES[i]);
    }

    // Mostrar resultados
    for (int i = 0; i < numLenguajes; i++) {
        printf("\n=== Lenguaje %d ===\n", i + 1);
        mostrarLista(LENGUAJES[i]);
    }

    // Liberar memoria
    for (int i = 0; i < numLenguajes; i++) {
        liberarLista(LENGUAJES[i]);
        free(LENGUAJES[i]);
    }
    free(LENGUAJES);

    return 0;
}

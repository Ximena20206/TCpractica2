#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <locale.h>

#define MAX_NOMBRE 100
#define MAX_PALABRA 100
#define MAX_LENGUAJE 100

// ----------------------------------------------------------
// ESTRUCTURA DE LA LISTA DOBLE PARA GUARDAR LAS PALABRAS
// ----------------------------------------------------------
typedef struct Nodo {
    char palabra[MAX_PALABRA];
    struct Nodo *anterior;
    struct Nodo *siguiente;
} Nodo;

typedef struct {
    Nodo *inicio;
    Nodo *fin;
} ListaDoble;

// ----------------------------------------------------------
// FUNCIONES DE LA LISTA DOBLE
// ----------------------------------------------------------
void inicializarLista(ListaDoble *lista){
    lista->inicio = NULL;
    lista->fin = NULL;
}

// Inserta una palabra en orden alfabético y por longitud
void insertarOrdenado(ListaDoble *lista, const char *palabra){
    Nodo *nuevo = (Nodo*)malloc(sizeof(Nodo));
    strcpy(nuevo->palabra,palabra);
    nuevo->anterior=nuevo->siguiente=NULL;

    if(lista->inicio==NULL){
        lista->inicio=lista->fin=nuevo;
        return;
    }

    Nodo *actual = lista->inicio;
    while(actual){
        if(strlen(palabra)<strlen(actual->palabra) ||
          (strlen(palabra)==strlen(actual->palabra) && strcmp(palabra,actual->palabra)<0)){
            nuevo->siguiente=actual;
            nuevo->anterior=actual->anterior;

            if(actual->anterior) actual->anterior->siguiente=nuevo;
            else lista->inicio=nuevo;

            actual->anterior=nuevo;
            return;
        }
        actual=actual->siguiente;
    }

    nuevo->anterior=lista->fin;
    lista->fin->siguiente=nuevo;
    lista->fin=nuevo;
}

void mostrarLista(ListaDoble *lista){
    Nodo *actual=lista->inicio;
    while(actual){
        printf("%s ",actual->palabra);
        actual=actual->siguiente;
    }
    printf("\n");
}

void liberarLista(ListaDoble *lista){
    Nodo *actual = lista->inicio;
    while(actual){
        Nodo *temp=actual;
        actual=actual->siguiente;
        free(temp);
    }
    lista->inicio=lista->fin=NULL;
}

int existePalabra(ListaDoble *lista, const char *palabra) {
    Nodo *actual = lista->inicio;
    while (actual != NULL) {
        if (strcmp(actual->palabra, palabra) == 0)
            return 1;  // La palabra ya existe en la lista
        actual = actual->siguiente;
    }
    return 0;  // No se encontró
}

void cargarDesdeArchivo(ListaDoble *lista, const char *nombreArchivo) {
    setlocale(LC_ALL, "");  // Permite acentos si los hay
    FILE *archivo = fopen(nombreArchivo, "r");

    if (!archivo) {
        printf("No se pudo abrir el archivo %s\n", nombreArchivo);
        exit(1);
    }

    char palabra[MAX_PALABRA];
    char ultimaPalabra[MAX_PALABRA] = "";

    while (fscanf(archivo, "%99s", palabra) == 1) {
        for (char *p = palabra; *p; ++p) {
            if (*p >= 'A' && *p <= 'Z') *p += 32; 
        }

        if (strcmp(palabra, ultimaPalabra) != 0) {

            if (!existePalabra(lista, palabra)) {
                insertarOrdenado(lista, palabra);
            }
            strcpy(ultimaPalabra, palabra);
        }
    }

    fclose(archivo);
}

// ----------------------------------------------------------
// FUNCIONES DE APOYO PARA OPERACIONES DE LENGUAJES
// ----------------------------------------------------------

// Copiar un arreglo de cadenas
char **copiarLenguaje(char **a,int tama){
    char **copia = (char**)malloc(tama*sizeof(char*));
    for(int i=0;i<tama;i++)
        copia[i]=strdup(a[i]);
    return copia;
}

// Concatenar dos cadenas
char *concatenarcadena(char *a,char *b){
    int tamA=strlen(a),tamB=strlen(b);
    char *c=(char*)malloc(tamA+tamB+1);
    if(!c){ printf("Error de memoria\n"); exit(0);}
    strcpy(c,a);
    strcat(c,b);
    return c;
}

// Concatenar dos lenguajes
char **concatenar(char **A,char **B,int n,int m,int *tam_res){
    char **C=(char**)malloc(n*m*sizeof(char*));
    int k=0;
    for(int i=0;i<n;i++)
        for(int j=0;j<m;j++)
            C[k++]=concatenarcadena(A[i],B[j]);
    *tam_res=k;
    return C;
}

// Unión de dos lenguajes
char **unions(char **A,char **B,int n,int m,int *tam_res){
    char **C=(char**)malloc((n+m)*sizeof(char*));
    int k=0,bandera;
    for(int i=0;i<n;i++)
        C[k++]=strdup(A[i]);
    for(int i=0;i<m;i++){
        bandera=0;
        for(int j=0;j<n;j++)
            if(strcmp(B[i],A[j])==0){bandera=1; break;}
        if(!bandera) C[k++]=strdup(B[i]);
    }
    *tam_res=k;
    return C;
}

// Reflexión de un lenguaje
char *inversa(char *a){
    int tam=strlen(a);
    char *c=(char*)malloc(tam+1);
    for(int i=0;i<tam;i++)
        c[i]=a[tam-1-i];
    c[tam]='\0';
    return c;
}
char **reflexion(char **A,int n,int *tam_res){
    char **res=(char**)malloc(n*sizeof(char*));
    for(int i=0;i<n;i++)
        res[i]=inversa(A[i]);
    *tam_res=n;
    return res;
}

// Potencia de un lenguaje
char **potencia(char **A,int n,int exponente,int *tam_res){
    if(exponente==0){
        char **res=(char**)malloc(1*sizeof(char*));
        res[0]=strdup("_"); // cadena vacía
        *tam_res=1;
        return res;
    }

    char **aux=copiarLenguaje(A,n);
    int aux_n=n;

    for(int k=1;k<exponente;k++){
        int nuevo_tam;
        char **temp=concatenar(aux,A,aux_n,n,&nuevo_tam);
        for(int i=0;i<aux_n;i++) free(aux[i]);
        free(aux);
        aux=temp;
        aux_n=nuevo_tam;
    }
    *tam_res=aux_n;
    return aux;
}

// Cerradura de Kleene
char **cerradura_klene(char **A,int n,int *tam_res){
    char *epsilon[]={"_"};
    char **res=copiarLenguaje(epsilon,1);
    *tam_res=1;
    for(int exp=1;exp<=4;exp++){
        int tamPot;
        char **pot=potencia(A,n,exp,&tamPot);
        int tamUnion;
        char **temp=unions(res,pot,*tam_res,tamPot,&tamUnion);
        for(int i=0;i<*tam_res;i++) free(res[i]);
        free(res);
        for(int i=0;i<tamPot;i++) free(pot[i]);
        free(pot);
        res=temp;
        *tam_res=tamUnion;
    }
    return res;
}

// Cerradura positiva
char **cerradura_positiva(char **A,int n,int *tam_res){
    char **res=NULL;
    *tam_res=0;
    for(int exp=1;exp<=4;exp++){
        int tamPot;
        char **pot=potencia(A,n,exp,&tamPot);
        int tamUnion;
        char **temp=unions(res,pot,*tam_res,tamPot,&tamUnion);
        for(int i=0;i<*tam_res;i++) free(res[i]);
        free(res);
        for(int i=0;i<tamPot;i++) free(pot[i]);
        free(pot);
        res=temp;
        *tam_res=tamUnion;
    }
    return res;
}

// Guardar arreglo de cadenas en archivo .txt
void guardarEnArchivo(char **L,int n,const char *nombreArchivo){
    FILE *f=fopen(nombreArchivo,"w");
    if(!f){printf("No se pudo crear archivo.\n"); return;}
    for(int i=0;i<n;i++)
        fprintf(f,"%s\n",L[i]);
    fclose(f);
    printf("Resultado guardado en '%s'\n",nombreArchivo);
}

// ----------------------------------------------------------
// PROGRAMA PRINCIPAL
// ----------------------------------------------------------
int main(){//int argc,char *argv[]){
    //sif(argc!=4){printf("Uso: %s <archivo_L1> <archivo_L2> <archivo_L3>\n",argv[0]); return 1;}
    char archivo1[MAX_NOMBRE], archivo2[MAX_NOMBRE], archivo3[MAX_NOMBRE];
    ListaDoble L1,L2,L3;
    Sleep(5000); 
        system("cls");

    int opcion=0;
    while(opcion!=8){
        inicializarLista(&L1);
        inicializarLista(&L2);
        inicializarLista(&L3);
        printf("=== CARGA DE LISTAS ===\n");
        printf("Ingrese el nombre del archivo para L1: ");
        scanf("%99s", archivo1);

        printf("Ingrese el nombre del archivo para L2: ");
        scanf("%99s", archivo2);

        printf("Ingrese el nombre del archivo para L3: ");
        scanf("%99s", archivo3);

        // Carga los archivos en las listas
        cargarDesdeArchivo(&L1, archivo1);
        cargarDesdeArchivo(&L2, archivo2);
        cargarDesdeArchivo(&L3, archivo3);

        // Muestra las listas
        printf("\n--- L1 ---\n");
        mostrarLista(&L1);

        printf("\n--- L2 ---\n");
        mostrarLista(&L2);

        printf("\n--- L3 ---\n");
        mostrarLista(&L3);
        
        printf("\n========= MENU =========\n");
        printf("1. Union\n2. Concatenacion\n3. Potencia\n4. Reflexion\n5. Cerradura de Kleene\n6. Cerradura positiva\n7. Mostrar lenguajes\n8. Salir\n");
        printf("Seleccione una opcion: "); scanf("%d",&opcion);

        char *L1_array[MAX_LENGUAJE],*L2_array[MAX_LENGUAJE],*L3_array[MAX_LENGUAJE];
        int n1=0,n2=0,n3=0;
        Nodo *a=L1.inicio; while(a){L1_array[n1++] = a->palabra; a=a->siguiente;}
        a=L2.inicio; while(a){L2_array[n2++] = a->palabra; a=a->siguiente;}
        a=L3.inicio; while(a){L3_array[n3++] = a->palabra; a=a->siguiente;}

        int nr=0;
        char salida[100];
        char nombreArchivo[150];

        switch(opcion){
            case 1: { // Union
                int lang1,lang2;
                printf("Seleccione lenguaje 1 (1=L1,2=L2,3=L3): "); scanf("%d",&lang1);
                printf("Seleccione lenguaje 2 (1=L1,2=L2,3=L3): "); scanf("%d",&lang2);
                char **A= (lang1==1?L1_array:(lang1==2?L2_array:L3_array));
                int nA = (lang1==1?n1:(lang1==2?n2:n3));
                char **B= (lang2==1?L1_array:(lang2==2?L2_array:L3_array));
                int nB = (lang2==1?n1:(lang2==2?n2:n3));
                char **res=unions(A,B,nA,nB,&nr);
                printf("Union: "); for(int i=0;i<nr;i++){printf("%s ",res[i]);} printf("\n");
                printf("Archivo salida: "); scanf("%s",salida);
                snprintf(nombreArchivo,sizeof(nombreArchivo),"%s.txt",salida);
                guardarEnArchivo(res,nr,nombreArchivo);
                for(int i=0;i<nr;i++) free(res[i]);
                free(res);
                break;
            }
            case 2: { // Concatenacion
                int lang1,lang2;
                printf("Seleccione lenguaje 1 (1=L1,2=L2,3=L3): "); scanf("%d",&lang1);
                printf("Seleccione lenguaje 2 (1=L1,2=L2,3=L3): "); scanf("%d",&lang2);
                char **A= (lang1==1?L1_array:(lang1==2?L2_array:L3_array));
                int nA = (lang1==1?n1:(lang1==2?n2:n3));
                char **B= (lang2==1?L1_array:(lang2==2?L2_array:L3_array));
                int nB = (lang2==1?n1:(lang2==2?n2:n3));
                char **res=concatenar(A,B,nA,nB,&nr);
                printf("Concatenacion: "); for(int i=0;i<nr;i++){printf("%s ",res[i]);} printf("\n");
                printf("Archivo salida: "); scanf("%s",salida);
                snprintf(nombreArchivo,sizeof(nombreArchivo),"%s.txt",salida);
                guardarEnArchivo(res,nr,nombreArchivo);
                for(int i=0;i<nr;i++) free(res[i]);
                free(res);
                break;
            }
            case 3: { // Potencia
                int lang,p;
                printf("Seleccione lenguaje a operar (1=L1,2=L2,3=L3): "); scanf("%d",&lang);
                char **A= (lang==1?L1_array:(lang==2?L2_array:L3_array));
                int nA = (lang==1?n1:(lang==2?n2:n3));
                printf("Ingrese potencia (-5 a 10): "); scanf("%d",&p);
                if(p<-5)p=-5; if(p>10)p=10;
                char **res=potencia(A,nA,p,&nr);
                printf("Potencia: "); for(int i=0;i<nr;i++){printf("%s ",res[i]);} printf("\n");
                printf("Archivo salida: "); scanf("%s",salida);
                snprintf(nombreArchivo,sizeof(nombreArchivo),"%s.txt",salida);
                guardarEnArchivo(res,nr,nombreArchivo);
                for(int i=0;i<nr;i++) free(res[i]);
                free(res);
                break;
            }
            case 4: { // Reflexion
                int lang;
                printf("Seleccione lenguaje a operar (1=L1,2=L2,3=L3): "); scanf("%d",&lang);
                char **A= (lang==1?L1_array:(lang==2?L2_array:L3_array));
                int nA = (lang==1?n1:(lang==2?n2:n3));
                char **res=reflexion(A,nA,&nr);
                printf("Reflexion: "); for(int i=0;i<nr;i++){printf("%s ",res[i]);} printf("\n");
                printf("Archivo salida: "); scanf("%s",salida);
                snprintf(nombreArchivo,sizeof(nombreArchivo),"%s.txt",salida);
                guardarEnArchivo(res,nr,nombreArchivo);
                for(int i=0;i<nr;i++) free(res[i]);
                free(res);
                break;
            }
            case 5: { // Cerradura de Kleene
                int lang;
                printf("Seleccione lenguaje a operar (1=L1,2=L2,3=L3): "); scanf("%d",&lang);
                char **A= (lang==1?L1_array:(lang==2?L2_array:L3_array));
                int nA = (lang==1?n1:(lang==2?n2:n3));
                char **res=cerradura_klene(A,nA,&nr);
                printf("Cerradura de Kleene: "); for(int i=0;i<nr;i++){printf("%s ",res[i]);} printf("\n");
                printf("Archivo salida: "); scanf("%s",salida);
                snprintf(nombreArchivo,sizeof(nombreArchivo),"%s.txt",salida);
                guardarEnArchivo(res,nr,nombreArchivo);
                for(int i=0;i<nr;i++) free(res[i]);
                free(res);
                break;
            }
            case 6: { // Cerradura positiva
                int lang;
                printf("Seleccione lenguaje a operar (1=L1,2=L2,3=L3): "); scanf("%d",&lang);
                char **A= (lang==1?L1_array:(lang==2?L2_array:L3_array));
              int nA = (lang==1 ? n1 : (lang==2 ? n2 : n3));
                char **res=cerradura_positiva(A,nA,&nr);
                printf("Cerradura positiva: "); for(int i=0;i<nr;i++){printf("%s ",res[i]);} printf("\n");
                printf("Archivo salida: "); scanf("%s",salida);
                snprintf(nombreArchivo,sizeof(nombreArchivo),"%s.txt",salida);
                guardarEnArchivo(res,nr,nombreArchivo);
                for(int i=0;i<nr;i++) free(res[i]);
                free(res);
                break;
            }
            case 7:
                printf("--- L1 ---\n"); mostrarLista(&L1);
                printf("--- L2 ---\n"); mostrarLista(&L2);
                printf("--- L3 ---\n"); mostrarLista(&L3);
                break;
            case 8:
                printf("Saliendo...\n"); break;
            default:
                printf("Opcion invalida.\n");
        }
        Sleep(5000); 
        system("cls");

        liberarLista(&L1);
        liberarLista(&L2);
        liberarLista(&L3);
    }

    

    return 0;
}

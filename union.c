#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int longitud_de_cadena(char *a){//FUNCION LONGITUD CADENA
    int i = 0;
    while(a[i] != '\0' && a[i] != '\n'){ 
        i++;
    }
    return i;
}

char* concatenarcadena(char *a,char* b){
    char* c;
    int tamA=longitud_de_cadena(a),tamB=longitud_de_cadena(b);
    int tamTotal=tamA+tamB;
 c = (char*)malloc((tamTotal + 1) * sizeof(char)); 
            if(c == NULL){
                printf("Error: no se pudo asignar memoria\n");
                exit(0);
            }
            for(int i = 0; i < tamA; i++)
                c[i] = a[i];
            
            for(int j = 0; j < tamB; j++)
                c[tamA + j] = b[j];
            

            c[tamTotal] = '\0'; 
          
            
            
            return c;

}
char** concatenar(char**a,char**b,int n,int m,int *tam_res){
char **c;
    int k = 0, bandera, i;

    c = (char**)malloc((m*n) * sizeof(char*));
    

  

    //Aqui revisaremos si algun elemento de b esta en a para no agregarlo y solo agregar los propios de B
    for(i=0; i<n; i++){
        bandera = 0;
        for(int j=0; j<m; j++){
            c[k]=concatenarcadena(a[i],b[j]);
            k++;
        }
        
    }

     *tam_res = k; 
    return c;


}
char** interseccion(char**a,char**b,int n,int m,int *tam_res){

   
int tam_ch,tam_gr,i,k=0;
    char **c;
    if(n<m){
    tam_ch=n;
     tam_gr=m;
    }
    
    else{
         tam_ch=m;
          tam_gr=n;

    }

    c = (char**)malloc(tam_ch * sizeof(char*));
    for(i = 0; i < tam_ch; i++)
        c[i] = (char*)malloc(50 * sizeof(char)); 

    // Buscar intersección
    for(i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            if(strcmp(a[i], b[j]) == 0) {
                strcpy(c[k], a[i]);
                k++;
                break; 
            }
        }
    }

    *tam_res = k; // Cuantos elementos hay en el lenguaje resultante
    return c;
  
}
char** unions(char **a, char **b, int m, int n, int *tam_res) {
    char **c;
    int k = 0, bandera, i;

    c = (char**)malloc((m+n) * sizeof(char*));
    for(i=0; i<(m+n); i++)
        c[i] = (char*)malloc(50 * sizeof(char));

   //primero vamos a pasar todos los elementos del lenguaje 1 al lenguaje resultante
    for(i=0; i<m; i++){
        strcpy(c[k], a[i]);
        k++;
    }

    //Aqui revisaremos si algun elemento de b esta en a para no agregarlo y solo agregar los propios de B
    for(i=0; i<n; i++){
        bandera = 0;
        for(int j=0; j<m; j++){
            if(strcmp(a[j], b[i]) == 0){
                bandera = 1;
                break;
            }
        }
        if(bandera == 0){
            strcpy(c[k], b[i]);
            k++;
        }
    }

    *tam_res = k; // Cuantos elementos hay en el lenguaje resultante
    return c;
}

int main(){
    char *a[] = {"_","0","1","10","11"};
    char *b[] = {"_","1","0110","11010"};   
    int tamunion,taminter,tamconca;
    char **c;
c = unions(a, b, sizeof(a)/sizeof(a[0]), sizeof(b)/sizeof(b[0]), &tamunion);
    printf("Union:\n{");
    for(int i=0; i<tamunion; i++){
        printf("%s,", c[i]);
        free(c[i]);
    }
     free(c);
     printf("}\n");
    c = interseccion(a, b, sizeof(a)/sizeof(a[0]), sizeof(b)/sizeof(b[0]), &taminter);
      printf("Interseccion:\n{");
    for(int i = 0; i < taminter; i++) {
        printf("%s,", c[i]);
        free(c[i]);
    }
    printf("}\n");
 free(c);
 c = concatenar(a, b, sizeof(a)/sizeof(a[0]), sizeof(b)/sizeof(b[0]),&tamconca);
  printf("Concatencacion:\n{");
    for(int i = 0; i < tamconca; i++) {
        printf("%s,", c[i]);
        free(c[i]);
    }
    printf("}\n");
 free(c);
    return 0;
}
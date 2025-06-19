#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tdas/list.h"
#include "tdas/map.h"

typedef struct{
    short alturaCm ; //altura del usuario en centimetros
    short pesoKg ; //peso del usuario en kilogramos
    short edad ;
    char sexo ; // 'M' masculino, 'F' femenino
    short objetivo ; // 1:ganar masa muscular, 2:perder grasa, 3:recomposicion corporal
} Usuario ;
typedef struct{
    int valorNutricional ;
    float proteinas ; //en gramos
    float carbohidratos ; //en gramos
    float grasas ; //en gramos
    float fibra ; //en gramos
} Alimento ;


int is_equal_string(const char *str1, const char *str2)
{ 
    return strcmp(str1, str2) == 0;
}

Map *ReadCsv_AddToMap(const char *nombreArchivo)
{
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) 
    {
        perror("Error al abrir el archivo");
        return NULL;
    }
    Map *mapaDeAlimentos = map_create(is_equal_string);
    char linea[256];
    fgets(linea, sizeof(linea), archivo); 
    while (fgets(linea, sizeof(linea), archivo))
    {
        char *aux = strtok(linea, ",");
        if (aux == NULL) continue; 

        char* nombreAlimento = strdup(aux);
        Alimento *alimento = malloc(sizeof(Alimento));
        if (alimento == NULL) 
        {
            perror("Error al asignar memoria para alimento");
            free(nombreAlimento);
            fclose(archivo);
            return NULL;
        }
        alimento->valorNutricional = atoi(strtok(NULL, ","));
        alimento->carbohidratos = atof(strtok(NULL, ","));
        alimento->proteinas = atof(strtok(NULL, ","));
        alimento->grasas = atof(strtok(NULL, ","));
        alimento->fibra = atof(strtok(NULL, ","));
        map_insert(mapaDeAlimentos, nombreAlimento, alimento);
    }
    fclose(archivo);
    return mapaDeAlimentos;
}

float calcularIMC(Usuario *usuario) 
{
    float alturaMetros = usuario->alturaCm / 100.0f;
    return usuario->pesoKg / (alturaMetros * alturaMetros);
}


void mostrarAlimentosPorPagina(Map *mapaAlimentos)
{
    int contador = 0;
    int numPagina = 1;
    MapPair *pair = map_first(mapaAlimentos);
    while (pair != NULL)
    {
        char *nombreAlimento = (char *)pair->key;
        Alimento *alimento = (Alimento *)pair->value;

        printf("Alimento: %s\n", nombreAlimento);
        printf("Calorias: %d\n", alimento->valorNutricional);
        printf("Proteinas: %.2f g\n", alimento->proteinas);
        printf("Carbohidratos: %.2f g\n", alimento->carbohidratos);
        printf("Grasas: %.2f g\n", alimento->grasas);
        printf("Fibra: %.2f g\n", alimento->fibra);
        printf("-------------------------\n");
        contador++;
        

        if (contador % 5 == 0) 
        {
            printf("Pagina %d\n", numPagina);
            printf("Presione Enter para continuar...\n");
            getchar(); // Espera a que el usuario presione Enter
            numPagina++;
        }
        {
            printf("Pagina %d\n", numPagina);

        }
        pair = map_first(mapaAlimentos);
    }
    if (contador % 5 != 0) 
    {
        printf("Pagina %d\n", numPagina);
    }
    printf("Fin de la lista de alimentos.\n");

}

void menufitApp() 
{
    
}

int main() {

    return 0 ;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tdas/list.h"
#include "tdas/map.h"

typedef struct
{
    short alturaCm; // altura del usuario en centimetros
    short pesoKg;   // peso del usuario en kilogramos
    short edad;
    char sexo;      // 'M' masculino, 'F' femenino
    short objetivo; // 1:ganar masa muscular, 2:perder grasa, 3:recomposicion corporal
    float imc;
    int caloriasDiarias;
} Usuario;
typedef struct
{
    char nombre[100]; // <-- Agregado
    int valorNutricional;
    float proteinas;     // en gramos
    float carbohidratos; // en gramos
    float grasas;        // en gramos
    float fibra;         // en gramos
} Alimento;

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
        if (aux == NULL)
            continue;

        char *nombreAlimento = strtok(linea, ",");
        Alimento *alimento = malloc(sizeof(Alimento));
        if (alimento == NULL)
        {
            perror("Error al asignar memoria para alimento");
            fclose(archivo);
            return NULL;
        }
        strncpy(alimento->nombre, nombreAlimento, sizeof(alimento->nombre) - 1);
        alimento->nombre[sizeof(alimento->nombre) - 1] = '\0';
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

void ingresarDatosPersona(Usuario *persona)
{
    printf("Ingrese su altura en cm: ");
    scanf("%hd", &persona->alturaCm);
    printf("\nIngrese su peso en kg: ");
    scanf("%hd", &persona->pesoKg);
    printf("\nIngrese su edad: ");
    scanf("%hd", &persona->edad);
    printf("\nIngrese su sexo (M/F): ");
    scanf(" %c", &persona->sexo);
    printf("\nIngrese su objetivo (1: ganar masa muscular, 2: perder grasa, 3: recomposición corporal): ");
    scanf("%hd", &persona->objetivo);
    getchar(); // Limpiar el buffer de entrada
    persona->imc = (float)persona->pesoKg / ((float)persona->alturaCm / 100.0f * (float)persona->alturaCm / 100.0f);

    float caloriasBase;
    if (persona->sexo == 'M' || persona->sexo == 'm')
    {
        caloriasBase = 10 * persona->pesoKg + 6.25f * persona->alturaCm - 5 * persona->edad + 5;
    }
    else if (persona->sexo == 'F' || persona->sexo == 'f')
    {
        caloriasBase = 10 * persona->pesoKg + 6.25f * persona->alturaCm - 5 * persona->edad - 161;
    }

    switch (persona->objetivo)
    {
    case 1:
        persona->caloriasDiarias = caloriasBase + 500;
        break;
    case 2:
        persona->caloriasDiarias = caloriasBase - 500;
        break;
    case 3:
        persona->caloriasDiarias = caloriasBase;
        break;
    default:
        printf("Objetivo no válido. Se establecerá a mantenimiento.\n");
        persona->caloriasDiarias = caloriasBase;
        break;
    }
    printf("\nDatos ingresados:\n");
    printf("Altura: %hd cm\n", persona->alturaCm);
    printf("Peso: %hd kg\n", persona->pesoKg);
    printf("Edad: %hd años\n", persona->edad);
    printf("Sexo: %c\n", persona->sexo);
    printf("Objetivo: %hd\n", persona->objetivo);
    printf("IMC: %.2f\n", persona->imc);
    printf("Calorías diarias recomendadas: %d\n", persona->caloriasDiarias);
}

void mostrarAlimentosPorPagina(Map *mapaAlimentos)
{
    int contador = 0;
    int numPagina = 1;
    int porPagina;
    printf("Ingrese la cantidad de alimentos que desea ver por pagina: ");
    scanf("%d", &porPagina);
    getchar();

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

        if (contador % porPagina == 0)
        {
            printf("Pagina %d\n", numPagina);
            printf("Presione Enter para continuar...\n");
            getchar(); // Espera a que el usuario presione Enter
            numPagina++;
        }
        pair = map_next(mapaAlimentos);
    }
    if (contador % porPagina != 0)
    {
        printf("Pagina %d\n", numPagina);
    }
    printf("Fin de la lista de alimentos.\n");
}
void verHistorialAlimentos(List *historial)
{
    if (list_is_empty(historial))
    {
        printf("No hay alimentos en el historial.\n");
        return;
    }
    void *dato = list_first(historial);
    while (dato != NULL)
    {
        Alimento *alimento = (Alimento *)dato;
        printf("Alimento: %s\n", alimento->nombre);
        printf("Calorias: %d\n", alimento->valorNutricional);
        printf("Proteinas: %.2f g\n", alimento->proteinas);
        printf("Carbohidratos: %.2f g\n", alimento->carbohidratos);
        printf("Grasas: %.2f g\n", alimento->grasas);
        printf("Fibra: %.2f g\n", alimento->fibra);
        printf("-------------------------\n");
        dato = list_next(historial);
    }
}
void eliminacionUltimaComida(List **historial)
{
    if (list_is_empty(*historial))
    {
        printf("No hay alimentos en el historial para eliminar.\n");
        return;
    }
    Alimento *ultimoAlimento = (Alimento *)list_remove_last(*historial);
    if (ultimoAlimento != NULL)
    {
        printf("Se ha eliminado el alimento: %s\n", ultimoAlimento->nombre);
        free(ultimoAlimento);
    }
    else
    {
        printf("Error al eliminar el último alimento.\n");
    }
    return;
}
void agregarComidaPropia(Map **mapaAlimentos)
{
    Alimento *nuevoAlimento = malloc(sizeof(Alimento));
    if (nuevoAlimento == NULL)
    {
        perror("Error al asignar memoria para nuevo alimento");
        return;
    }
    printf("Ingrese el nombre del alimento: ");
    fgets(nuevoAlimento->nombre, sizeof(nuevoAlimento->nombre), stdin);
    nuevoAlimento->nombre[strcspn(nuevoAlimento->nombre, "\n")] = 0; // Eliminar el salto de línea

    printf("Ingrese el valor nutricional (calorías): ");
    scanf("%d", &nuevoAlimento->valorNutricional);
    printf("Ingrese la cantidad de proteínas (g): ");
    scanf("%f", &nuevoAlimento->proteinas);
    printf("Ingrese la cantidad de carbohidratos (g): ");
    scanf("%f", &nuevoAlimento->carbohidratos);
    printf("Ingrese la cantidad de grasas (g): ");
    scanf("%f", &nuevoAlimento->grasas);
    printf("Ingrese la cantidad de fibra (g): ");
    scanf("%f", &nuevoAlimento->fibra);

    map_insert(*mapaAlimentos, nuevoAlimento->nombre, nuevoAlimento);
    printf("Nuevo alimento agregado exitosamente.\n");
    return;
}
void menufitApp()
{
    printf("Bienvenido a la AppFit!\n");
    int opcion;
    do
    {
        printf("Menu:\n");
        printf("1. Ingresar datos personales\n");
        printf("2.Cargar alimentos desde CSV\n");
        printf("3. Ver alimentos\n");
        printf("4.Ver historial de alimentos\n");
        print("5. Planificar plan semanmal\n");
        printf("6. Eliminar ultima comida ingerida\n");
        printf("7. Conteo de calorias y meta diaria\n");
        printf("8.Agregar comida propia\n");
        printf("9. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        getchar(); // Limpiar el buffer de entrada

    } while (opcion != 9);
    Usuario usuario;
    Map *mapaAlimentos = NULL;
    List *historialAlimentos = list_create();
    if (historialAlimentos == NULL)
    {
        printf("Error al crear el historial de alimentos.\n");
        return;
    }
    switch (opcion)
    {
    case 1:
        ingresarDatosPersona(&usuario);
        break;
    case 2:
        mapaAlimentos = ReadCsv_AddToMap("alimentos.csv");
        if (mapaAlimentos == NULL)
        {
            printf("Error al cargar los alimentos.\n");
        }
        else
        {
            printf("Alimentos cargados exitosamente.\n");
        }
        break;
    case 3:
        if (mapaAlimentos != NULL)
        {
            mostrarAlimentosPorPagina(mapaAlimentos);
        }
        else
        {
            printf("No hay alimentos cargados.\n");
        }
        break;
    case 4:
        verHistorialAlimentos(historialAlimentos);
        break;
    case 5:
        printf("Planificación semanal aún no implementada.\n");
        break;
    case 6:
        eliminacionUltimaComida(&historialAlimentos);
        break;
    case 7:
        conteoCaloriasYMetaDiaria(historialAlimentos, &usuario);
        break;
    case 8:
        agregarComidaPropia(&mapaAlimentos);
        break;
    case 9:
        printf("Saliendo de la AppFit. ¡Hasta luego!\n");
        return;

    default:
        printf("Opcion no valida.\n");
    }
}

int main()
{

    return 0;
}
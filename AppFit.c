#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
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

int is_equal_string(void *str1, void *str2)
{
    return strcmp((const char*)str1, (const char*)str2) == 0;
}

void printfRojo(const char *mensaje) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
    printf("%s", mensaje);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);   
}

void printfVerde(const char *mensaje) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("%s", mensaje);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);   
}

void printfProgresivo(const char *mensaje, int delay) {
    while (*mensaje) {
        putchar(*mensaje++);
        fflush(stdout);
        Sleep(delay); // Delay en milisegundos
    }
    printf("\n");
}

void printfProgresivoColor(const char *mensaje, WORD color, int delay) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) return;

    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    WORD originalColor = consoleInfo.wAttributes;

    SetConsoleTextAttribute(hConsole, color);

    for (int i = 0; mensaje[i] != '\0'; i++) {
        putchar(mensaje[i]);
        fflush(stdout);
        Sleep(delay); // Delay en milisegundos
    }

    SetConsoleTextAttribute(hConsole, originalColor);
    putchar('\n');
}

char* limpiarComillas(char* token) 
{
    if (token == NULL) return NULL;
    size_t len = strlen(token);
    if (len >= 2 && token[0] == '"' && token[len - 1] == '"') 
    {
        token[len - 1] = '\0';  
        return token + 1;       
    }
    return token;
}


Map *ReadCsv_AddToMap(const char *nombreArchivo)
{
    FILE *archivo = fopen(nombreArchivo, "r");
    if (!archivo) {
        perror("Error al abrir el archivo");
        return NULL;
    }

    Map *mapaDeAlimentos = map_create(is_equal_string);
    char linea[512];

    fgets(linea, sizeof(linea), archivo); // Saltar encabezado

    while (fgets(linea, sizeof(linea), archivo))
    {
        Alimento *alimento = malloc(sizeof(Alimento));
        if (!alimento) {
            perror("Memoria insuficiente");
            fclose(archivo);
            return NULL;
        }

        char *token = strtok(linea, ",");
        if (!token) continue;
        strncpy(alimento->nombre, token, sizeof(alimento->nombre));
        alimento->nombre[strcspn(alimento->nombre, "\n")] = 0;

        token = strtok(NULL, ",");
        alimento->valorNutricional = token ? atoi(limpiarComillas(token)) : 0;

        token = strtok(NULL, ",");
        alimento->carbohidratos = token ? atof(limpiarComillas(token)) : 0.0;

        token = strtok(NULL, ",");
        alimento->proteinas = token ? atof(limpiarComillas(token)) : 0.0;

        token = strtok(NULL, ",");
        alimento->grasas = token ? atof(limpiarComillas(token)) : 0.0;

        token = strtok(NULL, ",\n");
        alimento->fibra = token ? atof(limpiarComillas(token)) : 0.0;

        map_insert(mapaDeAlimentos, strdup(alimento->nombre), alimento);
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
    printf("\nIngrese su objetivo (1: ganar masa muscular, 2: perder grasa, 3: recomposicion corporal): ");
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
        printf("Objetivo no valido. Se establecera a mantenimiento.\n");
        persona->caloriasDiarias = caloriasBase;
        break;
    }
    printf("\nDatos ingresados:\n");
    printf("Altura: %hd cm\n", persona->alturaCm);
    printf("Peso: %hd kg\n", persona->pesoKg);
    printf("Edad: %hd anyos\n", persona->edad);
    printf("Sexo: %c\n", persona->sexo);
    printf("Objetivo: %hd\n", persona->objetivo);
    printf("IMC: %.2f\n", persona->imc);
    printf("Calorias diarias recomendadas: %d\n", persona->caloriasDiarias);
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
    if (list_size(historial) == 0)
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
    if (list_size(*historial) == 0)
    {
        printf("No hay alimentos en el historial para eliminar.\n");
        return;
    }
    Alimento *ultimoAlimento = (Alimento *)list_popBack(*historial);
    if (ultimoAlimento != NULL)
    {
        printf("Se ha eliminado el alimento: '%s'\n", ultimoAlimento->nombre);
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
    printf("Ingrese la cantidad de proteinas (g): ");
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

void agregarComidaConsumida(List *historial, Map *mapaAlimentos)
{
    char nombreAlimento[100];
    printf("Ingrese el nombre del alimento consumido: ");
    fgets(nombreAlimento, sizeof(nombreAlimento), stdin); 
    nombreAlimento[strcspn(nombreAlimento, "\n")] = 0; 

    MapPair *pair = map_search(mapaAlimentos, nombreAlimento);
    if (pair == NULL) {
        printf("El alimento '%s' no se encuentra en el mapa de alimentos. \n", nombreAlimento);
        return;
    }
    Alimento *alimento = (Alimento *)pair->value;
    Alimento *nuevoAlimento = malloc(sizeof(Alimento));
    if (nuevoAlimento == NULL) {
        printf("Error al asignar memoria para el nuevo alimento.\n");
        exit(EXIT_FAILURE);
    }
    *nuevoAlimento = *alimento; // Copiar los datos del alimento encontrado
    list_pushBack(historial, nuevoAlimento);
    printf("Alimento '%s' agregado al historial de alimentos correctamente.\n", nombreAlimento);
}

void menufitApp()
{
    WORD rojo = FOREGROUND_RED | FOREGROUND_INTENSITY;
    WORD verde = FOREGROUND_GREEN | FOREGROUND_INTENSITY;

    Usuario usuario;
    Map *mapaAlimentos = NULL;
    List *historialAlimentos = list_create();
    if (historialAlimentos == NULL)
    {
        printf("Error al crear el historial de alimentos.\n");
        return;
    }

    printfProgresivo("Bienvenido a FitFuel!\n\n", 10);
    int opcion;
    do
    {
        printfProgresivo("Menu Principal\n", 1);
        printfProgresivo("1. Ingresar datos personales", 1);
        printfProgresivo("2. Cargar alimentos desde CSV", 1);
        printfProgresivo("3. Ver alimentos", 1);
        printfProgresivo("4. Ver historial de alimentos", 1);
        printfProgresivo("5. Planificar plan semanmal", 1);
        printfProgresivo("6. Eliminar ultima comida ingerida", 1);
        printfProgresivo("7. Conteo de calorias y meta diaria", 1);
        printfProgresivo("8. Agregar comida propia", 1);
        printfProgresivo("9. Agregar comida consumida", 1);
        printfProgresivo("10. Salir", 1);
        printfProgresivo("Seleccione una opcion: ", 1);
        scanf("%d", &opcion);
        getchar(); // Limpiar el buffer de entrada


        switch (opcion)
        {
            case 1:
                system("cls||clear");
                ingresarDatosPersona(&usuario);
                break;
            case 2:
                system("cls||clear");
                mapaAlimentos = ReadCsv_AddToMap("tabla_de_alimentos.csv");

                if (mapaAlimentos == NULL)
                {
                    printfProgresivoColor("Error al cargar los alimentos.\n", rojo, 5);
                }
                else
                {
                    printfProgresivoColor("Alimentos cargados exitosamente.\n", verde, 5);
                }
                break;
            case 3:
                system("cls||clear");
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
                system("cls||clear");
                verHistorialAlimentos(historialAlimentos);
                break;
            case 5:
                system("cls||clear");
                printfRojo("Planificacion semanal aun no implementada.\n");
                break;
            case 6:
                system("cls||clear");
                eliminacionUltimaComida(&historialAlimentos);
                break;
            case 7:
                system("cls||clear");
                //conteoCaloriasYMetaDiaria(historialAlimentos, &usuario);
                break;
            case 8:
                system("cls||clear");
                agregarComidaPropia(&mapaAlimentos);
                break;
            case 9:
                system("cls||clear");
                agregarComidaConsumida(historialAlimentos, mapaAlimentos);
                break;
            case 10:
                system("cls||clear");
                printf("Saliendo de FitFuel. ¡Hasta luego!\n");
                return;

            default:
                printfRojo("Opcion no valida.\n");
        }
    } while (opcion != 10);
}

int main()
{
    menufitApp();
    return 0;
}
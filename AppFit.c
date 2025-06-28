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

typedef struct
{
    char dia[15];
    List *alimentosPorDia;

} planPorDia;

typedef struct
{
    planPorDia dias[7];

} planSemanal;

WORD rojo = FOREGROUND_RED | FOREGROUND_INTENSITY;
WORD verde = FOREGROUND_GREEN | FOREGROUND_INTENSITY;

int is_equal_string(void *str1, void *str2)
{
    return strcmp((const char *)str1, (const char *)str2) == 0;
}

void printfRojo(const char *mensaje)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
    printf("%s", mensaje);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void printfVerde(const char *mensaje)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    printf("%s", mensaje);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void printfProgresivo(const char *mensaje, int delay)
{
    while (*mensaje)
    {
        putchar(*mensaje++);
        fflush(stdout);
        Sleep(delay); // Delay en milisegundos
    }
    printf("\n");
}

void printfProgresivoColor(const char *mensaje, WORD color, int delay)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE)
        return;

    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    WORD originalColor = consoleInfo.wAttributes;

    SetConsoleTextAttribute(hConsole, color);

    for (int i = 0; mensaje[i] != '\0'; i++)
    {
        putchar(mensaje[i]);
        fflush(stdout);
        Sleep(delay); // Delay en milisegundos
    }

    SetConsoleTextAttribute(hConsole, originalColor);
    putchar('\n');
}

// Función para limpiar comillas del archivo CSV
char *limpiarComillas(char *token)
{
    if (token == NULL)
        return NULL;
    size_t len = strlen(token);
    if (len >= 2 && token[0] == '"' && token[len - 1] == '"')
    {
        token[len - 1] = '\0';
        return token + 1;
    }
    return token;
}

/* Esta funcion se encarga de abrir el archivo CSV para luego crear el mapa de alimentos e irle asignando cada dato de los alimentos a la parte de su estructura correspondiente, asi leyendolos
de manera correcta*/
Map *ReadCsv_AddToMap(const char *nombreArchivo)
{
    FILE *archivo = fopen(nombreArchivo, "r");
    if (!archivo)
    {
        perror("Error al abrir el archivo");
        return NULL;
    }
    // Crear un mapa para almacenar los alimentos
    Map *mapaDeAlimentos = map_create(is_equal_string);
    char linea[512];

    fgets(linea, sizeof(linea), archivo); // Saltar encabezado
    // Leer cada línea del archivo CSV
    while (fgets(linea, sizeof(linea), archivo))
    {
        Alimento *alimento = malloc(sizeof(Alimento));
        if (!alimento)
        {
            perror("Memoria insuficiente");
            fclose(archivo);
            return NULL;
        }

        char *token = strtok(linea, ",");
        if (!token)
            continue;
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

/* Esta funcion se encarga de leer y guardar en la estructura los datos de la persona ademas que a traves de la formula de Harris-Benedict nos calcula la cantidad de calorias que deberiamos
consumir en base al objetivo que queremos cumplir, agregandolo a la estructura correspondiente, ademas de calcular nuestro IMC*/
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
    // Calcular las calorías diarias recomendadas
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
    printf("Calorias diarias recomendadas: %d\n\n", persona->caloriasDiarias);
}

/* Lo que hace esta funcion es preguntarnos cuantos alimentos queremos ver por pagina, para luego recorrer todo el mapa de alimentos e ir mostrandonos los alimentos con todos sus datos
nutricionales correspondientes.*/
void mostrarAlimentosPorPagina(Map *mapaAlimentos)
{
    int contador = 0;
    int numPagina = 1;
    int porPagina;
    printf("Ingrese la cantidad de alimentos que desea ver por pagina: \n");
    scanf("%d", &porPagina);
    getchar();

    MapPair *pair = map_first(mapaAlimentos);
    // Verifica si el mapa está vacío
    // si es así, muestra un mensaje y retorna
    // de lo contrario, continúa mostrando los alimentos
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
    printf("\nFin de la lista de alimentos.\n\n");
}

/* Lo que hace esta funcion es recorrer la lista del historial de alimentos consumidos y mostrarnos con todos sus valores nutricionales los alimentos que hemos consumido, si es que no hemos
consumido alimentos la funcion nos mostrara un mensaje personalizado avisandonos que no hay alimentos en el historial*/
void verHistorialAlimentos(List *historial)
{
    // Verifica si el historial está vacío
    if (list_size(historial) == 0)
    {
        printf("No hay alimentos en el historial.\n");
        return;
    }
    void *dato = list_first(historial);
    // Recorre el historial de alimentos
    while (dato != NULL)
    {
        Alimento *alimento = (Alimento *)dato;
        printf("Alimento: %s\n", alimento->nombre);
        printf("Calorias: %d\n", alimento->valorNutricional);
        printf("Proteinas: %.2f g\n", alimento->proteinas);
        printf("Carbohidratos: %.2f g\n", alimento->carbohidratos);
        printf("Grasas: %.2f g\n", alimento->grasas);
        printf("Fibra: %.2f g\n", alimento->fibra);
        printf("-------------------------\n\n");
        dato = list_next(historial);
    }
}

/* Lo que realiza esta funcion es crear un lista que dentro de esta contiene otra lista con los dias de la semana para asi crear el plan semanal y poder personalizarlo.*/
planSemanal *crearPlanSemanal()
{
    // Asigna memoria para el plan semanal y los días de la semana
    planSemanal *plan = malloc(sizeof(planSemanal));
    char *nombresDias[] = {"Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado", "Domingo"};
    // Inicializa los días de la semana
    for (int i = 0; i < 7; i++)
    {
        // Asigna memoria para cada día y su lista de alimentos
        strcpy(plan->dias[i].dia, nombresDias[i]);
        plan->dias[i].alimentosPorDia = list_create();
    }
    return plan;
}

/* Lo que hace esta funcion es que agrega al plan semanal especificamente al dia de la semana los alimentos que consumiremos*/
void agregarAlimentoADia(planSemanal *plan, int diaIndex, Alimento *alimento)
{
    if (diaIndex < 0 || diaIndex > 6)
        return;
    Alimento *copia = malloc(sizeof(Alimento));
    *copia = *alimento;
    list_pushBack(plan->dias[diaIndex].alimentosPorDia, copia);
}

/* Lo que realiza esta funcion es en base al plan semanal que tenemos y los alimentos ingresados, crear un archivo tipo .CSV el cual contendra todos nuestros alimentos previamente 
planificados, este archivo se alojara en el directorio donde tenemos el codigo.*/
void exportarPlanSemanalCSV(planSemanal *plan, const char *nombreArchivo)
{
    FILE *archivo = fopen(nombreArchivo, "w");
    if (!archivo)
    {
        perror("No se pudo crear el archivo");
        return;
    }

    fprintf(archivo, "Dia,Alimento,Calorias,Proteinas,Carbohidratos,Grasas,Fibra\n");

    for (int i = 0; i < 7; i++)
    {
        planPorDia *dia = &plan->dias[i];
        void *nodo = list_first(dia->alimentosPorDia);
        while (nodo != NULL)
        {
            Alimento *a = (Alimento *)nodo;
            fprintf(archivo, "%s,%s,%d,%.2f,%.2f,%.2f,%.2f\n",
                    dia->dia,
                    a->nombre,
                    a->valorNutricional,
                    a->proteinas,
                    a->carbohidratos,
                    a->grasas,
                    a->fibra);
            nodo = list_next(dia->alimentosPorDia);
        }
    }

    fclose(archivo);
    printf("\n Plan semanal guardado en '%s'\n", nombreArchivo);
}

/* Lo que hace esta funcion es eliminar el ultimo alimento consumido, lo realiza ingresando a la lista del historial de comidas consumidas para luego realizar un popBack e informandonos
el alimento que se elimino*/
void eliminacionUltimaComida(List **historial)
{
    // Verifica si el historial está vacío
    if (list_size(*historial) == 0)
    {
        printf("No hay alimentos en el historial para eliminar.\n");
        return;
    }
    // Elimina el último alimento del historial
    Alimento *ultimoAlimento = (Alimento *)list_popBack(*historial);
    if (ultimoAlimento != NULL)
    {
        printf("Se ha eliminado el alimento: '%s'\n\n", ultimoAlimento->nombre);
        free(ultimoAlimento);
    }
    else
    {
        printf("Error al eliminar el ultimo alimento.\n");
    }
    return;
}

/* Lo que realiza esta funcion es ingresar a nuestro historial de alimentos y mostrarnos los alimentos que hemos consumido en el dia junto a sus valores nutricionales para luego en base 
al objetivo de kiloCalorias que tenemos propuesto, calcular y mostrarnos si es que nos faltan calorias y cuantas, si alcanzamos la meta de manera exacta o si alcanzamos la meta con 
un excedente correspondiente el cual tambien nos muestra*/
void conteoCaloriasYMetaDiaria(List *historialAlimentos, Usuario *usuario)
{
    if (list_size(historialAlimentos) == 0)
    {
        printfProgresivoColor("No hay alimentos en el historial.\n", rojo, 5);
        return;
    }

    printfProgresivo("Calculando conteo de calorias y meta diaria...\n", 1);
    Sleep(2000);

    float totalProteinas = 0, totalCarbohidratos = 0, totalGrasas = 0, totalFibra = 0;
    int totalCalorias = 0;
    // Recorre el historial de alimentos y suma los valores nutricionales
    Alimento *dato = (Alimento *)list_first(historialAlimentos);
    while (dato != NULL)
    {
        totalCalorias += dato->valorNutricional;
        totalProteinas += dato->proteinas;
        totalCarbohidratos += dato->carbohidratos;
        totalGrasas += dato->grasas;
        totalFibra += dato->fibra;
        dato = (Alimento *)list_next(historialAlimentos);
    }

    char buffer[128];
    // Imprime el conteo de calorías y la meta diaria
    printfProgresivo("Conteo de calorias y meta diaria:\n", 5);
    sprintf(buffer, "Calorias diarias recomendadas: %d", usuario->caloriasDiarias);
    printfProgresivo(buffer, 5);

    printfProgresivoColor("Analisis:\n", verde, 5);
    // Imprime los totales de cada macronutriente
    sprintf(buffer, "Total de calorias consumidas: %d", totalCalorias);
    printfProgresivo(buffer, 5);

    sprintf(buffer, "Total de proteinas consumidas: %.2f g", totalProteinas);
    printfProgresivo(buffer, 5);

    sprintf(buffer, "Total de carbohidratos consumidos: %.2f g", totalCarbohidratos);
    printfProgresivo(buffer, 5);

    sprintf(buffer, "Total de grasas consumidas: %.2f g", totalGrasas);
    printfProgresivo(buffer, 5);

    sprintf(buffer, "Total de fibra consumida: %.2f g\n", totalFibra);
    printfProgresivo(buffer, 5);

    // Verifica si se ha cumplido la meta de calorías diarias
    if (totalCalorias < usuario->caloriasDiarias)
    {
        int faltan = usuario->caloriasDiarias - totalCalorias;
        printfProgresivoColor("No se ha cumplido la meta de calorias diarias.", rojo, 5);
        sprintf(buffer, "Faltan %d calorias para alcanzar tu meta diaria.\n", faltan);
        printfProgresivoColor(buffer, rojo, 5);
    }
    else
    {
        printfProgresivoColor("Meta de calorias diarias cumplida!", verde, 5);
        int excedente = totalCalorias - usuario->caloriasDiarias;
        sprintf(buffer, "Te excediste por %d calorias.", excedente);
        printfProgresivoColor(buffer, verde, 5);
    }
}

/* Lo que hace esta funcion es crear un nuevo alimento al cual nosotros debermos ingresarle nombre junto a todos sus valores nutricionales correspondientes para luego agregarlo al mapa de 
alimentos para asi poder utilizarlo en las demas funciones.*/
void agregarComidaPropia(Map **mapaAlimentos)
{
    // Verifica si el mapa de alimentos está inicializado
    Alimento *nuevoAlimento = malloc(sizeof(Alimento));
    if (nuevoAlimento == NULL)
    {
        perror("Error al asignar memoria para nuevo alimento");
        return;
    }
    // Solicita al usuario los datos del nuevo alimento
    printf("Ingrese el nombre del alimento: ");
    fgets(nuevoAlimento->nombre, sizeof(nuevoAlimento->nombre), stdin);
    nuevoAlimento->nombre[strcspn(nuevoAlimento->nombre, "\n")] = 0; // Eliminar el salto de línea

    printf("Ingrese el valor nutricional (calorias): ");
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
    printf("Nuevo alimento agregado exitosamente.\n\n");
    return;
}

/* Lo que realiza esta funcion es que a traves de una busqueda por palabra clave, recorre el mapa y busca los nombres de alimentos que contengan esta palabra clave para luego mostrarnos
los alimentos que coinciden con esta palabra clave, dejandonos con indices numericos para agregar a la lista del historial de comidas consumidas.*/
void agregarComidaConsumida(List *historial, Map *mapaAlimentos)
{
    // Verifica si el mapa de alimentos está inicializado
    char consulta[100];
    printf("Ingrese parte del nombre del alimento consumido: ");
    fgets(consulta, sizeof(consulta), stdin);
    consulta[strcspn(consulta, "\n")] = 0;
    // Busca alimentos que coincidan con la consulta
    List *coincidencias = list_create();
    MapPair *pair = map_first(mapaAlimentos);
    while (pair != NULL)
    {
        Alimento *a = (Alimento *)pair->value;
        if (strstr(a->nombre, consulta) != NULL)
        {
            list_pushBack(coincidencias, a);
        }
        pair = map_next(mapaAlimentos);
    }
    // Si no se encontraron coincidencias, muestra un mensaje y retorna
    if (list_size(coincidencias) == 0)
    {
        printfRojo("No se encontraron alimentos que coincidan con la busqueda.\n");
        list_clean(coincidencias);
        return;
    }
    // Muestra las coincidencias encontradas
    printfVerde("\nResultados encontrados:\n\n");
    int i = 0;
    void *nodo = list_first(coincidencias);
    while (nodo != NULL)
    {
        Alimento *a = (Alimento *)nodo;
        printf("[%d] %s - %d kcal\n", i, a->nombre, a->valorNutricional);
        nodo = list_next(coincidencias);
        i++;
    }

    int indice = -1;
    printf("\nSeleccione el numero del alimento que desea agregar al historial: ");
    scanf("%d", &indice);
    getchar();

    if (indice < 0 || indice >= list_size(coincidencias))
    {
        printfRojo("Indice invalido. No se agrego ningun alimento.\n");
        list_clean(coincidencias);
        return;
    }

    Alimento *seleccionado = list_first(coincidencias);
    for (int j = 0; j < indice; j++)
    {
        seleccionado = list_next(coincidencias);
    }

    Alimento *copia = malloc(sizeof(Alimento));
    *copia = *seleccionado;
    list_pushBack(historial, copia);
    printfVerde("Alimento agregado al historial correctamente.\n\n");

    list_clean(coincidencias);
}

/* Esta funcion se encarga de asignarle un indice numerico a cada alimento que muestra el programa al momento de anadir comidas al historial diario consumido o al plan semanal,
facilitando el uso de este.*/
int mostrarAlimentosConIndice(Map *mapaAlimentos, Alimento **buffer)
{
    int index = 0;
    int porPagina = 10;
    int contador = 0;
    MapPair *pair = map_first(mapaAlimentos);
    while (pair != NULL)
    {
        Alimento *alimento = (Alimento *)pair->value;
        // Muestra el alimento con su índice
        printf("%d. %s - Calorias: %d\n", index + 1, alimento->nombre, alimento->valorNutricional);
        buffer[index] = alimento;
        index++;
        contador++;

        if (contador % porPagina == 0)
        {
            printf("Presione Enter para ver mas...\n");
            getchar();
        }

        pair = map_next(mapaAlimentos);
    }

    return index; // Devuelve cuántos alimentos se mostraron
}

/* Lo que hace esta funcion es que por una palabra clave ingresada por el usuario recorre todo el mapa buscando los alimentos que contengan esa palabra clave y mostrarlos, esto facilita
las funciones de agregar comidas consumidas y agregar comidas a nuestro plan semanal*/
int buscarAlimentosPorNombre(Map *mapaAlimentos, Alimento **buffer, const char *busqueda)
{
    int index = 0;
    MapPair *pair = map_first(mapaAlimentos);
    while (pair != NULL)
    {
        // Verifica si el alimento contiene la palabra clave
        Alimento *alimento = (Alimento *)pair->value;
        // Si el nombre del alimento contiene la palabra clave, lo muestra
        if (strstr(alimento->nombre, busqueda) != NULL)
        {
            printf("%d. %s - Calorias: %d\n", index + 1, alimento->nombre, alimento->valorNutricional);
            buffer[index++] = alimento;
        }
        pair = map_next(mapaAlimentos);
    }

    return index;
}

/* Lo que realiza esta funcion es planificar la comida de la semana, nos pregunta cuantos alimentos queremos agregar por dia para luego seleccionar los alimentos que querrarmos consumir
en esos dias correspondientes, para luego exportarnos el archivo csv de nuestro plan semanal.*/
void planificarComidasSemanal(Map *mapaAlimentos, planSemanal *plan)
{
    for (int i = 0; i < 7; i++)
    {
        // Solicita al usuario la cantidad de alimentos a agregar para el día
        printfVerde(plan->dias[i].dia);
        printf(" Cuantos alimentos desea agregar para este dia?: ");
        int cantidad;
        scanf("%d", &cantidad);
        getchar();
        for (int j = 0; j < cantidad; j++)
        {
            char termino[100];
            Alimento *buffer[1000];
            // Muestra los alimentos disponibles por palabra clave
            printf("\nIngrese una palabra clave para buscar el alimento #%d de %s: ", j + 1, plan->dias[i].dia);
            fgets(termino, sizeof(termino), stdin);
            termino[strcspn(termino, "\n")] = 0;
            int total = 0;
            MapPair *pair = map_first(mapaAlimentos);
            while (pair != NULL)
            {
                Alimento *alimento = (Alimento *)pair->value;
                if (strstr(alimento->nombre, termino) != NULL)
                {
                    printf("%d. %s - Calorias: %d\n", total + 1, alimento->nombre, alimento->valorNutricional);
                    buffer[total++] = alimento;
                }
                pair = map_next(mapaAlimentos);
            }

            if (total == 0)
            {
                printfRojo("No se encontraron alimentos con esa palabra. Intente con otra.\n");
                j--;
                continue;
            }

            printf("Seleccione el numero del alimento que desea agregar: \n");
            int seleccion;
            scanf("%d", &seleccion);
            getchar();

            if (seleccion < 1 || seleccion > total)
            {
                printfRojo("Seleccion invalida. Intente nuevamente.\n");
                j--;
                continue;
            }

            agregarAlimentoADia(plan, i, buffer[seleccion - 1]);
        }
    }

    exportarPlanSemanalCSV(plan, "plan_semanal.csv");
    printfVerde("\nPlan semanal creado y exportado exitosamente!\n");
}

/* Esta funcion lo que realiza es que al momento de iniciar el programa nos pregunta si queremos importar el plan semanal, lo cual lo que hace es cargar el plan semanal que creamos
anteriormente para asi poder utilizarlo y si es necesario actualizarlo.*/
planSemanal *importarPlanSemanalDesdeCSV(const char *nombreArchivo)
{
    FILE *archivo = fopen(nombreArchivo, "r");
    if (!archivo)
    {
        perror("No se pudo abrir el archivo de plan semanal");
        return NULL;
    }
    // Crea un nuevo plan semanal
    planSemanal *plan = crearPlanSemanal();
    char linea[512];

    fgets(linea, sizeof(linea), archivo);
    while (fgets(linea, sizeof(linea), archivo))
    {
        char *token = strtok(linea, ",");
        if (!token)
            continue;

        char dia[20];
        strncpy(dia, token, sizeof(dia));
        dia[sizeof(dia) - 1] = '\0';

        token = strtok(NULL, ",");
        if (!token)
            continue;
        Alimento *a = malloc(sizeof(Alimento));
        strncpy(a->nombre, token, sizeof(a->nombre));
        a->nombre[strcspn(a->nombre, "\n")] = 0;

        token = strtok(NULL, ",");
        a->valorNutricional = token ? atoi(token) : 0;

        token = strtok(NULL, ",");
        a->proteinas = token ? atof(token) : 0.0;

        token = strtok(NULL, ",");
        a->carbohidratos = token ? atof(token) : 0.0;

        token = strtok(NULL, ",");
        a->grasas = token ? atof(token) : 0.0;

        token = strtok(NULL, ",\n");
        a->fibra = token ? atof(token) : 0.0;

        for (int i = 0; i < 7; i++)
        {
            if (strcmp(plan->dias[i].dia, dia) == 0)
            {
                list_pushBack(plan->dias[i].alimentosPorDia, a);
                break;
            }
        }
    }

    fclose(archivo);
    return plan;
}

/* Lo que realiza esta funcion es recorrer la lista del plan semanal dia por dia para mostrarnos el nombre de los alimentos sus valores nutricionales ademas de calcularnos las calorias que 
hay por dia, y tambien las calorias totales que contiene este plan semanal*/
void mostrarPlanSemanal(planSemanal *plan)
{
    int caloriasSemana = 0;

    for (int i = 0; i < 7; i++)
    {
        // Muestra el día de la semana
        planPorDia *dia = &plan->dias[i];
        printfVerde(dia->dia);
        printf(":\n");
        // Verifica si hay alimentos registrados para el día
        if (list_size(dia->alimentosPorDia) == 0)
        {
            printf("  (Sin alimentos registrados)\n\n");
            continue;
        }

        int caloriasTotalesDia = 0;
        void *nodo = list_first(dia->alimentosPorDia);
        while (nodo != NULL)
        {
            Alimento *a = (Alimento *)nodo;
            printf("  - %s (%d kcal)\n", a->nombre, a->valorNutricional);
            caloriasTotalesDia += a->valorNutricional;
            nodo = list_next(dia->alimentosPorDia);
        }

        printf("  Calorias totales del dia: %d kcal\n\n", caloriasTotalesDia);
        caloriasSemana += caloriasTotalesDia;
    }

    printf("\n Calorias totales de la semana: %d kcal\n\n", caloriasSemana);
}

// Función para mostrar el menú de la aplicación FitFuel
void menufitApp()
{
    planSemanal *plan = NULL;

    // Solicita al usuario si desea cargar un plan semanal anterior
    printf("Deseas cargar tu plan semanal anterior? (S/N): ");
    char respuesta;
    scanf(" %c", &respuesta);
    getchar();

    if (respuesta == 'S' || respuesta == 's')
    {
        plan = importarPlanSemanalDesdeCSV("plan_semanal.csv");
        if (plan == NULL)
        {
            // Si no se pudo cargar el plan, crea uno nuevo
            printfRojo("No se pudo cargar el plan anterior. Se creara uno nuevo.\n");
            plan = crearPlanSemanal();
        }
        else
        {
            // Si se cargó correctamente, muestra un mensaje de éxito
            printfVerde("Plan semanal anterior cargado exitosamente.\n");
        }
    }
    else
    {
        plan = crearPlanSemanal();
    }

    Usuario usuario;
    Map *mapaAlimentos = NULL;
    List *historialAlimentos = list_create();

    if (historialAlimentos == NULL)
    {
        // Si no se pudo crear el historial de alimentos, muestra un mensaje de error y retorna
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
        printfProgresivo("4. Agregar comida propia", 1);
        printfProgresivo("5. Agregar comida consumida", 1);
        printfProgresivo("6. Eliminar ultima comida ingerida", 1);
        printfProgresivo("7. Conteo de calorias y meta diaria", 1);
        printfProgresivo("8. Ver historial de alimentos", 1);
        printfProgresivo("9. Planificar plan semanal", 1);
        printfProgresivo("10. Mostrar plan semanal", 1);
        printfProgresivo("11. Salir", 1);
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
            agregarComidaPropia(&mapaAlimentos);
            break;
        case 5:
            system("cls||clear");
            agregarComidaConsumida(historialAlimentos, mapaAlimentos);
            break;
        case 6:
            system("cls||clear");
            eliminacionUltimaComida(&historialAlimentos);
            break;
        case 7:
            system("cls||clear");
            conteoCaloriasYMetaDiaria(historialAlimentos, &usuario);
            break;
        case 8:
            system("cls||clear");
            verHistorialAlimentos(historialAlimentos);
            break;
        case 9:
            system("cls||clear");
            planificarComidasSemanal(mapaAlimentos, plan);
            break;
        case 10:
            system("cls||clear");
            if (plan == NULL)
            {
                printfRojo("No hay un plan semanal cargado.\n");
            }
            else
            {
                mostrarPlanSemanal(plan);
            }
            break;
        case 11:
            system("cls||clear");
            // Liberamos memoria
            list_clean(historialAlimentos);
            free(historialAlimentos);
            if (mapaAlimentos != NULL)
            {
                map_clean(mapaAlimentos);
                free(mapaAlimentos);
            }
            printf("Saliendo de FitFuel. Hasta luego!\n");
            return;

        default:
            printfRojo("Opcion no valida.\n");
        }
    } while (opcion != 11);
}

int main()
{
    menufitApp();
    return 0;
}
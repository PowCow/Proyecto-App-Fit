# Fit-Fuel 

## Descripción del Programa
Fit-Fuel es una aplicación que en base a nuestros datos fisicos es decir altura, peso, sexo, edad y objetivo que querramos cumplir (ganar masa muscular, perder grasa o recomposición corporal) nos dice cuántas calorías deberiamos consumir
para cumplir con nuestro objetivo. Además nos permite crear planes diarios o planes semanales es decir podemos ir agregando los alimentos que vamos consumiendo a lo largo del dia calculandonos cuantas calorías nos faltan y todos sus aportes
nutricionales, o támbien podemos crear un plan semanal, es decir que nosotros organicemos que comeremos en los 7 días de la semana para mayor organización con nuestras comidas, este nos mostrará el total de calorías de cada dia además de el total
de kiloCalorías que consumiremos esa semana. Para poder hacer esto tenemos un archivo *.CSV* base el cual contendrá multiples alimentos para poder ir agregando, además de que el programa al crear el plan semanal nos retornará un archivo del mismo 
tipo con nuestra planificación semanal, así permitiendonos poder cerrar el programa sin perder el plan y permitiendonos cargarlo apenas iniciemos el programa así tenemos mayor personalización de este. 

## Como ejecutar para utilizar el programa
Este programa ha sido desarrollado en el lenguaje C y puede ejecutarse utilizando *Visual Studio Code* junto con una extensión para C/C++, como la *Extension pack de Microsoft* que se descarga del mismo Visual Studio Code
Para poder ejecutarlo siga los siguientes pasos. **Cabe recalcar que este programa puede ser solamente ejecutado en Sistema Operativo Windows** debido a funcionalidades de este.

## Requisitos Previos
1. Tener Sistema Operativo Windows
2. Tener instalado Visual Studio Code
3. Tener la extención C/C++
4. Tener instalado un compilador de C, como por ejemplo *MinGW*

## Pasos para ejecutar
1. Descarga el archivo .zip y descomprime este en un directorio a elección
2. Abre el código en Visual Studio Code
Para esto:
- Abre la aplicacion
- Archivo -> Abrir Carpeta
- Y seleccionamos la carpeta donde descomprimimos el programa
3. Para compilar el codigo
- Crea una nueva terminal, en Terminal -> Nueva Terminal
- En la terminal coloca este comando: **gcc -o FitFuel AppFit.c tdas/list.c tdas/map.c**

*(Esto lo que hará será crear un Archivo.Exe para ejecutar el programa)
Luego ingresa a la carpeta donde hayas descomprimido el programa y ejecuta el .Exe*

## Funcionalidades

1. Ingresar datos personales, se ingresa nuestra estatura, peso en kg, edad, sexo y el objetivo que queremos cumplir, nos calcula el IMC y las kiloCalorías diarias que debemos consumir.
2. Cargar alimentos desde el CSV, Funcion para que el programa cargue el .csv de los alimentos.
3. Ver alimentos, se ingresa la cantidad de alimentos que queremos ver por pagina y la funcion nos muestra todos los alimentos ordenados de manera alfabetica con todos sus datos nutricionales. Presionamos Enter para pasar pagina.
4. Agregar comida propia, se ingresa el nombre de la comida personalizada que agregaremos, las calorias, proteinas, carbohidratos, grasas y fibras, y se agregará exitosamente en el csv de los alimentos.
5. Agregar comida consumida, se ingresa una palabra clave del nombre del alimento consumido, la funcion nos muestra todos los alimentos con esa palabra clave y sus numeros de indice para agregar al historial, seleccionamos el ingerido y este se agrega al historial
diario consumido
6. Eliminar ultima comida ingerida, esta funcion al utilizarla nos muestra la ultima comida ingerida y que fue eliminada de nuestro historial diario
7. Conteo de calorias y meta diaria, la funcion nos calcula el conteo de las calorias que llevamos en el dia, nos muestra cuantas calorias deberiamos consumir, para luego mostrarnos el total de calorias, proteinas, carbohidratos, grasas y fibras consumidos hasta
el momento, para luego mostrarnos si hemos cumplido la meta diaria o no, y en base a las calorías que deberiamos consumir nos muestra dependiendo del caso cuantas calorias nos faltan, si llegamos a la meta con las kiloCalorias exactas, o si llegamos a la meta y el excedente de estas
8. Ver historial de alimentos , esta funcion nos muestra los alimentos que hemos consumido en el dia con su nombre y todos sus valores nutricionales.
9. Planificar plan semanal, nos preguntara cuantos alimentos queremos en el dia que estemos planificando ej Lunes para luego buscando por la palabra clave agregar los alimentos que planificamos comer en ese dia, así sucesivamente con los 7 dias de la semana, luego de realizar la planificación, la función nos retornará un archivo CSV con el plan semanal que nosotros creamos.
10. Mostrar plan semanal, al momento de ejecutar esta opcion, la función nos mostrará todas las comidas que tenemos en cada dia de la semana junto a sus kiloCalorías además de las calorías totales de ese día, esto lo hará con los 7 días correspondientes para luego mostrarnos el total de las kiloCalorías de la semana, es decir la suma en cada día.
11. Salir, al hacer esto saldremos del programa liberando la memoria reservada.

## Problemas Conocidos:
1. Al momento de ingresar carácteres que no son validos con lo que pide el programa este fallará.
2. Al momento de realizar la busqueda de comida por palabra clave, el programa debe recorrer todo el mapa, siendo algo no tan óptimo.

## Cosas para mejorar
1. Evitar que el programa falle cuando se le ingresen carácteres incorrectos es decir letras en vez de numeros y viceversa.
2. Optimizar la búsqueda en el mapa para mayor eficiencia
3. Agregar otro csv con alimentos preparados y todos sus valores nutricionales.
4. Hacer que el programa guarde los datos de la persona y no ir ingresandolos cada vez que se ejecute.

## Ejemplo de uso 

> Al ingresar al programa, se le preguntará al usuario si quiere cargar su plan semanal, previamente creado. En este caso, no tenemos uno hecho por lo que seleccionaremos que no.

```
Deseas cargar tu plan semanal anterior? (S/N): N
Bienvenido a FitFuel!
```
> Como se seleccionó que no, el programa creará un plan semanal vacío.

1. Ingresar los datos del usuario: Opción seleccionada (1). Este proceso se realiza una única vez, cada vez que se inicia el programa, no se guarda como pasa con el plan semanal. Se ingresa los datos en el siguiente orden: Altura en centímetros, peso en kilogramos, edad, sexo (M para masculino o F para femenino) y por último el objetivo a cumplir. Al terminar de ingresar estos datos, se crearán nuevas variables, IMC y calorías recomendadas, ambas calculadas mediante el uso de fórmulas.
```
Menu Principal

1. Ingresar datos personales
2. Cargar alimentos desde CSV
3. Ver alimentos
4. Agregar comida propia
5. Agregar comida consumida
6. Eliminar ultima comida ingerida
7. Conteo de calorias y meta diaria
8. Ver historial de alimentos
9. Planificar plan semanal
10. Mostrar plan semanal
11. Salir
Seleccione una opcion: 
```
> Ingrese su altura en cm: 175
> 
> Ingrese su peso en kg: 75
> 
> Ingrese su edad: 19
> 
> Ingrese su sexo (M/F): M
> 
> Ingrese su objetivo (1: ganar masa muscular, 2: perder grasa, 3: recomposicion corporal): 3

```
Datos ingresados:
Altura: 175 cm
Peso: 75 kg
Edad: 19 anyos
Sexo: M
Objetivo: 3
IMC: 24.49
Calorias diarias recomendadas: 1753
```

2. Cargar alimentos desde el CSV: Opción seleccionada (2). El programa indicará por pantalla si es que se cargaron correctamente los alimentos, indicando también si es que no.

```
Alimentos cargados exitosamente.
```

3. Ver alimentos: Opción seleccionada (3). Los alimentos se mostrarán por páginas, donde el usuario ingresa el número de alimentos que desea ver por página. Para que el usuario pase de página, unicamente tendrá que presionar, enter. En el caso de querer terminar de ver los alimentos, puede mantener el enter hasta volver al menú principal.

```
Ingrese la cantidad de alimentos que desea ver por pagina: 3
Alimento: Abadejo ahumado
Calorias: 116
Proteinas: 25.23 g
Carbohidratos: 0.00 g
Grasas: 0.96 g
Fibra: 0.00 g
-------------------------
Alimento: Abulon al vapor o escalfado
Calorias: 209
Proteinas: 34.05 g
Carbohidratos: 11.97 g
Grasas: 1.51 g
Fibra: 0.00 g
-------------------------
Alimento: Acedias Fritas
Calorias: 256
Proteinas: 17.00 g
Carbohidratos: 12.00 g
Grasas: 16.00 g
Fibra: 0.00 g
-------------------------
Pagina 1
Presione Enter para continuar...
```

```
Pagina 254
Fin de la lista de alimentos.
```

4. Agrega comida propia: Opción seleccionada (4). En caso de no encontrar un alimento que consume el usuario, pero no está, el usuario puede ingresarlo por si mismo para usarlo más adelante. Para esto el usuario deberá ingresar el nombre del alimento junto a sus valores nutricionales, es importante tener en cuenta que estos valores son por 100 gramos de dicho alimento.

> Ingrese el nombre del alimento: Cebolla escabeche
> 
> Ingrese el valor nutricional (calorias): 18
> 
> Ingrese la cantidad de proteinas (g): 0.2
> 
> Ingrese la cantidad de carbohidratos (g): 3.23
> 
> Ingrese la cantidad de grasas (g): 0.02
> 
> Ingrese la cantidad de fibra (g): 0.3

```
Nuevo alimento agregado exitosamente.
```

5. Agregar comida consumida: Opción seleccionada (5). Permitirá al usuario ingresar el nombre de un alimento y mostrará si es que existen múltiples coincidencias con el nombre ingresado. Luego seleccionará el alimento deseado según un índice que se le creará a cada uno. 

> Ingrese parte del nombre del alimento consumido: Pan

```
Resultados encontrados:
[0] "Harina Trigo Panificada" - 337 kcal
[1] Pan blanco - 266 kcal
[2] "Pan blanco tostado" - 297 kcal
[3] "Pan centeno tostado" - 285 kcal
[4] Pan de nuez - 392 kcal
[5] Pan frances  - 272 kcal
[6] "Pan frances de trigo integral" - 239 kcal
[7] Pan integral - 252 kcal
[8] Pan multigrano - 265 kcal
[9] "Pan pita tostado" - 302 kcal
[10] Pan sin gluten - 248 kcal
[11] "Pan salvado de avena" - 299 kcal
Seleccione el numero del alimento que desea agregar al historial: 7
```

> Alimento agregado al historial correctamente.

6. Eliminar última comida ingerida: Opción seleccionada (6). En cualquier caso que el usuario se equivoque, y seleccione un número que no era, tiene la opción de inmediatamente borrar el último alimento que ingresó.

> Se ha eliminado el alimento: 'Pan integral'

7. Conteo de calorías y meta diaria: Opción seleccionada (7). Esta opción será un análisis a las comidas ingeridas del usuario ese día, por lo que usaremos varias veces la opción 5. Haremos cuenta de que hemos ingresado varios alimentos, para que se muestre un análisis contundente.

> Alimentos ingresados: Jurel, Burrito con carne, Arroz cocido con leche, Pan francés, Pavo, Zumo de coco, Nuggets de pollo, Macarrones, Leche entera, Puré de papa.

```
Calculando conteo de calorias y meta diaria...

Conteo de calorias y meta diaria:

Calorias diarias recomendadas: 1753
Analisis:

Total de calorias consumidas: 1756
Total de proteinas consumidas: 101.57 g
Total de carbohidratos consumidos: 211.49 g
Total de grasas consumidas: 57.99 g
Total de fibra consumida: 8.80 g
Meta de calorias diarias cumplida!
Te excediste por 3 calorias.
```

8. Ver historial de alimentos: Opción seleccionada (8). Mostrará los alimentos ingeridos en orden de consumo, si no hay alimentos en este historial, lo indicará por pantalla.

```
Alimento: Jurel
Calorias: 112
Proteinas: 15.40 g
Carbohidratos: 0.80 g
Grasas: 5.20 g
Fibra: 0.00 g
-------------------------
Alimento: Burrito con carne
Calorias: 242
Proteinas: 13.03 g
Carbohidratos: 14.44 g
Grasas: 14.48 g
Fibra: 1.20 g
-------------------------
Alimento: Arroz cocido con leche
Calorias: 143
Proteinas: 5.06 g
Carbohidratos: 25.03 g
Grasas: 2.21 g
Fibra: 0.30 g
-------------------------
Alimento: Pan frances
Calorias: 272
Proteinas: 10.75 g
Carbohidratos: 51.88 g
Grasas: 2.42 g
Fibra: 2.20 g
-------------------------
Alimento: Pavo
Calorias: 107
Proteinas: 21.90 g
Carbohidratos: 0.00 g
Grasas: 2.20 g
Fibra: 0.00 g
-------------------------
Alimento: Zumo de coco
Calorias: 31
Proteinas: 0.21 g
Carbohidratos: 2.92 g
Grasas: 2.08 g
Fibra: 0.00 g
-------------------------
Alimento: Nuggets de pollo
Calorias: 307
Proteinas: 15.92 g
Carbohidratos: 14.93 g
Grasas: 20.36 g
Fibra: 0.90 g
-------------------------
Alimento: Macarrones
Calorias: 370
Proteinas: 14.00 g
Carbohidratos: 79.00 g
Grasas: 2.00 g
Fibra: 3.00 g
-------------------------
Alimento: Leche entera
Calorias: 61
Proteinas: 3.15 g
Carbohidratos: 4.80 g
Grasas: 3.25 g
Fibra: 0.00 g
-------------------------
Alimento: Pure de papa
Calorias: 111
Proteinas: 2.15 g
Carbohidratos: 17.69 g
Grasas: 3.79 g
Fibra: 1.20 g
-------------------------
```

9. Planificar plan semanal: Opción seleccionada (9). Mostrará por pantalla el día al que se agregarán alimentos, de Lunes a Domingo. Pedirá al usuario ingresar el número de alimentos que desea agregar en dicho día, para el ejemplo, usaremos 2 alimentos para cada día y se mostrará solamente el día Lunes, para que se sepa como es el menú.

```
Lunes Cuantos alimentos desea agregar para este dia?: 2

Ingrese una palabra clave para buscar el alimento #1 de Lunes: Papas fritas
1. Papas fritas - Calorias: 529
Seleccione el numero del alimento que desea agregar: 1

Ingrese una palabra clave para buscar el alimento #2 de Lunes: Pechuga
1. "Pollo Pechuga asado" - Calorias: 175
2. "Pollo Pechugas Kentucky" - Calorias: 436
Seleccione el numero del alimento que desea agregar: 1
```

> Alimentos ingresados para el día Martes: Higado Pollo, Pure de papa.
> 
> Alimentos ingresados para el día Miercoles: Pure de papa, Carne caballo.
> 
> Alimentos ingresados para el día Jueves: Arroz Integral, Nuggets de pollo.
> 
> Alimentos ingresados para el día Viernes: Ñoquis papa, Zumo de uva 100%.
> 
> Alimentos ingresados para el día Sábado: Macarrones, Bebida energetica (Monster).
> 
> Alimentos ingresados para el día Domingo: Burrito de pollo, Bebida energetica (Rockstar).

```
Plan semanal guardado en 'plan_semanal.csv'

Plan semanal creado y exportado exitosamente!
```

10. Mostrar plan semanal: Opción seleccionada (10). Mostrará por pantalla el plan recientemente creado, en orden según día, indicando unicamente las calorías de los alimentos.

```
Lunes:
  - Papas fritas (529 kcal)
  - "Pollo Pechuga asado" (175 kcal)
  Calorias totales del dia: 704 kcal

Martes:
  - Higado Pollo (137 kcal)
  - Pure de papa (111 kcal)
  Calorias totales del dia: 248 kcal

Miercoles:
  - Pure de papa (111 kcal)
  - Carne Caballo (113 kcal)
  Calorias totales del dia: 224 kcal

Jueves:
  - Arroz Integral (357 kcal)
  - Nuggets de pollo (307 kcal)
  Calorias totales del dia: 664 kcal

Viernes:
  - "Ñoquis papa" (133 kcal)
  - "Zumo de uva 100%" (60 kcal)
  Calorias totales del dia: 193 kcal

Sabado:
  - Macarrones (370 kcal)
  - Bebida energetica (Monster) (47 kcal)
  Calorias totales del dia: 417 kcal

Domingo:
  - Burrito de pollo (242 kcal)
  - Bebida energetica (Rockstar) (58 kcal)
  Calorias totales del dia: 300 kcal


Calorias totales de la semana: 2750 kcal
```

11. Salir del programa: Opción seleccionada (11). Finaliza el programa, no muestra un menú, por lo que al ingresar '11' en el menú principal y presionar Enter, finalizará la ejecución.


## Aportes individuales

### Coordinador: Simón Martínez
Realice las funciones de lectura del csv, creación del mapa de alimentos, ingresar datos de persona, calculo de IMC, cantidad de calorías necesarias, función para eliminar comillas, crear plan semanal, 
búsqueda por índice numérico y palabra clave, función para exportar y crear el archivo csv del plan semanal, creación de función conteo de calorías y meta diaria, además de primera parte del readme. 

### Responsable de Integración y Constancia: Paulina Muñoz
Aporte en organización del CSV, escritura y eliminacion de datos  

### Gestor de calidad: Flavio Luna
   
  
### Comunicador: Samuel Astudillo
Aporte en comentarios del codigo

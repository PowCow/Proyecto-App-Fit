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

void menufitApp() {
    
}

int main() {

    return 0 ;
}
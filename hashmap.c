#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {

  int posicion = hash(key, map->capacity);
  int i = 0;

  while(map->buckets[posicion] != NULL)
    {
      if(is_equal(map->buckets[posicion]->key, key) == 1)
      {
        map->buckets[posicion]->value = value;
        map->current = posicion;
        return;
      }
      posicion = (posicion + 1) % map->capacity;
      i++;
    }
  map->buckets[posicion] = createPair(key, value);
  map->current = posicion;
  map->size++;
}

void enlarge(HashMap * map) {
  enlarge_called = 1; //no borrar (testing purposes)

  Pair** aux = map->buckets;
  map->capacity = map->capacity*2;
  map->buckets = (Pair**)malloc(sizeof(Pair*)*map->capacity);
  map->size = 0;

  for(int i = 0; i < map->capacity; i++)
      map->buckets[i] = NULL;

  for(int i = 0; i < map->capacity/2; i++)
    {
      if(aux[i] != NULL)
        insertMap(map, aux[i]->key, aux[i]->value);
    }

  free(aux);
}


HashMap * createMap(long capacity) {
  HashMap *mapa = (HashMap *) malloc(sizeof(HashMap));
  mapa->buckets = (Pair **) calloc(capacity, sizeof(Pair *));
  mapa->size = 0;
  mapa->capacity = capacity;
  mapa->current = -1;
  return mapa;
}

void eraseMap(HashMap * map,  char * key) {    

  int posicion = hash(key, map->capacity);
  int i = 0;
  while(map->buckets[posicion] != NULL)
    {
      if(is_equal(map->buckets[posicion]->key, key) == 1)
      {
        map->buckets[posicion]->key = NULL;
        map->size--;
        return;
      }
      posicion = (posicion + 1) % map->capacity;
      i++;
    }
}

Pair * searchMap(HashMap * map,  char * key) {   

  int posicion = hash(key, map->capacity);
  int i = 0;
  while(map->buckets[posicion] != NULL)
    {
      if(is_equal(map->buckets[posicion]->key, key) == 1)
      {
        map->current = posicion;
        return map->buckets[posicion];
      }
      posicion = (posicion + 1) % map->capacity;
      i++;
    }
    return NULL;
}

Pair * firstMap(HashMap * map) {

  int i = 0;

  while(i < map->capacity)
    {
      if(map->buckets[i] != NULL && map->buckets[i]->key != NULL)
      {
        map->current = i;
        return map->buckets[i];
      }
      i++;
    }

  return NULL;
  
}

Pair * nextMap(HashMap * map) {

  int i = map->current + 1;
  while(i < map->capacity)
    {
      if(map->buckets[i] != NULL)
      {
        map->current = i;
        return map->buckets[i];
        
      }
      i++;
      
    }
  return NULL;
  
}

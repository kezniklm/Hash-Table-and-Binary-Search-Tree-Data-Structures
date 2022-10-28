/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) 
{
  	int result = 1;
  	int length = strlen(key);
  	for (int i = 0; i < length; i++) 
	{
    	result += key[i];
  	}
  	return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) 
{
	if(table == NULL)
	{
        return;
    }
	for(int i = 0; i < HT_SIZE;i++)
	{
		(*table)[i] = NULL; 
	}
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key)
{
	if(table == NULL || key == NULL)
	{ 
        return NULL;
    }

	int i = get_hash(key);
	for(ht_item_t *temp_element = ((*table)[i]);temp_element != NULL;temp_element = temp_element->next)
	{
		if (temp_element->key == key) 
		{
      		return temp_element;
		}
	}
  	return NULL;
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) 
{
	if(table == NULL || key == NULL)
	{
        return;
    }
	ht_item_t *temp_element = ht_search(table, key);
	if(temp_element == NULL)
	{
		ht_item_t *new_element = (ht_item_t *) malloc(sizeof(ht_item_t));
		if(new_element == NULL) //Ošetrenie v prípade zlyhania alokácie
		{
			return; 
		}
		new_element->value = value;
		new_element->key = key;
		int i = get_hash(key);
		new_element->next = (*table)[i];
		(*table)[i] = new_element;
		return;
	}
	else temp_element->value = value;
}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) 
{
	if(table == NULL || key == NULL)
	{ 
        return NULL;
    }

  	ht_item_t *temp_element = ht_search(table, key);
  	if (temp_element != NULL) 
  	{ 
    	return (&(temp_element->value)); 
  	}
 	return NULL;
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) 
{
    if(table == NULL || key == NULL) 
	{ 
        return;
    }
	
	int hash = get_hash(key);
	ht_item_t *element = (*table)[hash];
    ht_item_t *previous_element;
	int first = 0;
    
    while (element)
	{
        if (strcmp(element->key,key) == 0)
		{
			if(first == 0)
			{
				(*table)[hash] = NULL; //Prvok bol jediný na indexe
			}
			else 
			{
				previous_element->next = element->next; //Na indexe je viacej prvkov
			}
            
            free(element);
            return;
        }
		first++;
        previous_element = element;
        element = element->next;
    }
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) 
{
	if(table == NULL)
	{ 
        return;
    }
	ht_item_t *temp_element;
  	ht_item_t *previous_element;
  	for(int i = 0; i < HT_SIZE; i++) 
	{ 
    	temp_element = (*table)[i];

		//postupné uvoľňovanie prvkov
    	while (temp_element)
		{ 
      		previous_element = temp_element;
      		temp_element = temp_element->next;
      		free(previous_element);
    	}
    (*table)[i] = NULL; //Uvedenie tabuľky do stavu po inicializácii
  }
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "hash.h"

#define HASH(key) ((key)&(pTab->bucketSize - 1))

#define hash_alloc      malloc
#define hash_free       free
#define hash_assert     assert


// insert 
int list_insert(tab_list_t* l, tab_item_t* item)
{
    tab_item_t **p = &l->head;
    while (*p != NULL)
    {
        if((*p)->key == item->key)
        {
            return RE_ERR;
        }
        p = &(*p)->next;
    }
    *p = item;
    (*p)->next = NULL;

    return RE_OK;
}

// serach 
tab_item_t* list_find(tab_list_t* l, int key)
{
    tab_item_t **p = &l->head;
    while (*p)
    {
        if(key == (*p)->key)
        {
            return (*p);
        }
        p = &(*p)->next;
    }
    
    return NULL;
}

static inline tab_item_t **find_indirect(tab_list_t* l , int key)
{
    tab_item_t **p = &l->head;
    while (*p)
    {
        if((*p)->key == key)
        {
            return p;
        }
        p = &(*p)->next;
    }
    return p;
}

// remove
int list_remove(tab_list_t* l, int key)
{
    tab_item_t **p = find_indirect(l, key);
    if(*p)
    {
        (*p = (*p)->next);
        return RE_OK;
    }

    return RE_ERR;
}

/**
 * @brief init a hash table 
 * @param keySize: only souport times of 2^n 
 * @param hashRate: key hash to space compress times of 2^hashRate, max hashRate are log2(keySize)
*/
hash_table_t* tab_init( int keySize, int hashRate)
{
    if(keySize < 0)
    {
        return NULL;
    }
    int bktSize = keySize >> hashRate;

    if(!bktSize)
    {
        return NULL;
    }

    if(bktSize & (bktSize-1))
    {
        return NULL;
    }

    // mem alloc
    hash_table_t* pTab = hash_alloc(sizeof(hash_table_t)
                         + sizeof(tab_list_t)*bktSize);
    pTab->size = 0;
    pTab->bucketSize = bktSize;
    memset(pTab->bucket, 0, sizeof(tab_list_t)*bktSize);
    return pTab;

}

/**
 * @brief get  item numbers in hash table 
 * @return item numbers
*/
int tab_size(hash_table_t* pTab)
{
    if(pTab)
    {
        return pTab->size;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief insert a item to table, if 2 items with same key, the last one will insert failed
 * @param pTab: hash table context pointer 
 * @param item: pointer to item need to insert
 * @return insert success: RE_OK  else: RE_ERR
*/
int item_insert(hash_table_t *pTab, tab_item_t* item)
{
    if(NULL == pTab)
    {
        return RE_ERR;
    }

    if(item->key < 0)
    {
        return RE_ERR;
    }

    // step1: hash 
    int hashIdx = HASH(item->key);
    tab_list_t* l = &pTab->bucket[hashIdx];

    // step2: insert 
    int re = list_insert(l, item);

    // step3: state update 
    if(RE_OK == re)
    {
        pTab->size ++;
    }

    return re;
}

/**
 * @brief search a item in hash table by key 
 * @param pTab: hash table context pointer 
 * @param key: key of item to search
 * @return tab_item_t: item struct pointer 
*/
tab_item_t* item_search(hash_table_t* pTab, int key)
{
    if(!pTab->size)
    {
        return NULL;
    }

    if(key < 0)
    {
        return NULL;
    }

    // step1: hash 
    int hashIdx = HASH(key);
    tab_list_t* l = &pTab->bucket[hashIdx];
    
    // step2: search 
    return list_find(l, key);
}

/**
 * @brief delete a item from hash table by key 
 * @param pTab: hash table context pointer
 * @param key: key of item to delete
 * @return delete sucess:RE_OK else:RE_ERR
*/
int item_delete(hash_table_t* pTab, int key)
{
    if(key < 0)
    {
        return RE_ERR;
    }

    // step1: hash 
    int hashIdx = HASH(key);
    tab_list_t* l = &pTab->bucket[hashIdx];
    if(!l)
    {
        return RE_ERR;
    }

    //remove 
    int re = list_remove(l, key);
    if(RE_OK == re)
    {
        pTab->size --;
    }

    return re;
}

/**
 * @brief  return the first item in this hash table 
 * @param pTab: hash table pointer 
 * @return item geted: pointer of first item  else:NULL
*/
tab_item_t* tab_item_first(hash_table_t* pTab)
{
    if(!pTab->size)
    {
        return NULL;
    }

    tab_list_t* l = NULL;
    for(int i = 0; i < pTab->bucketSize; i ++)
    {
        l = &pTab->bucket[i];
        if(l->head)
        {
            return l->head;
        }
    }

    return NULL;
}

/**
 * @brief deinit table after all item be deleted
 * @return no return 
*/
void tab_deinit(hash_table_t* pTab)
{
    if(pTab->size)
    {
        hash_assert(0);
    }

    hash_free(pTab);
}
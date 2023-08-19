#ifndef __HASH_H__
#define __HASH_H__

enum 
{
    RE_OK = 0,
    RE_ERR = -1
};

/**
 * keyObj of a node in hash table, include a item struct in the top of struct
 * just like :
 * typedef struct xxxx
 * {
 *  table_item_t itemObj;
 *  xxxxx;
 *  xxxxx;
 * 
 * }
*/
typedef struct tab_item_
{
    int key;
    struct tab_item_ *next;

}tab_item_t;

typedef struct tab_list_
{
    tab_item_t* head;
}tab_list_t;

/**
 * hash table context
*/
typedef struct hash_table_
{
    int size;               /* table size , node count */
    int bucketSize;         /* list numbers, set when init */
    tab_list_t bucket[0];

}hash_table_t;

hash_table_t* tab_init(int keySize, int hashRate);
int tab_size(hash_table_t* pTab);
int item_insert(hash_table_t *pTab, tab_item_t* item);
tab_item_t* item_search(hash_table_t* pTab, int key);
int item_delete(hash_table_t* pTab, int key);
void tab_deinit(hash_table_t* pTab);
tab_item_t* tab_item_first(hash_table_t* pTab);



#endif // __HASH_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "hash.h"


typedef struct hash_tab_test_
{
    tab_item_t item;
    int varTest;
}hash_tab_test_t;


void init_deinit_test()
{
    int keySize = 1024; // 2^10
    int bucketRate = 6; // list max item are 

    // init  fail 
    assert(NULL == tab_init(keySize - 10, bucketRate));
    assert(NULL == tab_init(keySize, bucketRate + 5));

    // init right 
    hash_table_t* pTab = tab_init(keySize, bucketRate);
    assert(pTab);
    assert(16 == pTab->bucketSize);
    tab_deinit(pTab);
}

void insert_test()
{
    // init 
    int keySize = 1024;     // 2^10
    int bucketRate = 5;     // list max item are 
    hash_table_t* pTab = tab_init(keySize, bucketRate);
    assert(pTab);
    assert(32 == pTab->bucketSize);

    // insert 
    hash_tab_test_t* itemInsert = NULL;
    for(int i = 0; i < keySize; i ++)
    {
        itemInsert = malloc(sizeof(hash_tab_test_t));
        itemInsert->item.key = i;
        itemInsert->item.next = NULL;
        itemInsert->varTest = keySize - i;
        tab_item_t* itemObj = (tab_item_t*)itemInsert;
        assert(RE_OK == item_insert(pTab, itemObj));
    }

    // repeat 
    for(int i = 0; i < keySize; i ++)
    {
        itemInsert = malloc(sizeof(hash_tab_test_t));
        itemInsert->item.key = i;
        itemInsert->item.next = NULL;
        itemInsert->varTest = keySize - i;
        tab_item_t* itemObj = (tab_item_t*)itemInsert;
        assert(RE_ERR == item_insert(pTab, itemObj));

        free(itemInsert);
    }

    assert(keySize == pTab->size);
}

void search_test()
{
    // init 
    int keySize = 1024;     // 2^10
    int bucketRate = 5;     // list max item are 
    hash_table_t* pTab = tab_init(keySize, bucketRate);
    assert(pTab);
    assert(32 == pTab->bucketSize);

    // insert 
    hash_tab_test_t* itemInsert = NULL;
    for(int i = 0; i < keySize; i ++)
    {
        itemInsert = malloc(sizeof(hash_tab_test_t));
        itemInsert->item.key = i;
        itemInsert->item.next = NULL;
        itemInsert->varTest = keySize - i;
        tab_item_t* itemObj = (tab_item_t*)itemInsert;
        assert(RE_OK == item_insert(pTab, itemObj));
    }

    // search in range 
    hash_tab_test_t* itemSearch = NULL;
    for(int i = 0; i < keySize; i ++)
    {
        itemSearch = (hash_tab_test_t*)item_search(pTab, i);
        assert(itemSearch);
        assert(itemSearch->varTest == (keySize - i));
    }

    // search out of range 
    for(int i = keySize + 1; i < keySize*2; i ++)
    {
        itemSearch = (hash_tab_test_t*)item_search(pTab, i);
        assert(!itemSearch);
    }
}

void delete_test()
{
    // init 
    int keySize = 1024;     // 2^10
    int bucketRate = 5;     // list max item are 
    hash_table_t* pTab = tab_init(keySize, bucketRate);
    assert(pTab);
    assert(32 == pTab->bucketSize);

    // insert 
    hash_tab_test_t* itemInsert = NULL;
    for(int i = 0; i < keySize; i ++)
    {
        itemInsert = malloc(sizeof(hash_tab_test_t));
        itemInsert->item.key = i;
        itemInsert->item.next = NULL;
        itemInsert->varTest = keySize - i;
        tab_item_t* itemObj = (tab_item_t*)itemInsert;
        assert(RE_OK == item_insert(pTab, itemObj));
    }

    // delete item not inserted
    for(int i = keySize + 1; i < keySize*2; i ++)
    {
        assert(RE_ERR == item_delete(pTab, i));
    }
    assert(keySize == pTab->size);

    // delete 
    for(int i = 0; i < keySize; i ++)
    {
        assert(RE_OK == item_delete(pTab, i));
        assert(pTab->size == (keySize - i));
    }

    assert(!pTab->size);
}

void first_item_get_test()
{
    // init 
    int keySize = 1024;     // 2^10
    int bucketRate = 5;     // list max item are 
    hash_table_t* pTab = tab_init(keySize, bucketRate);
    assert(pTab);
    assert(32 == pTab->bucketSize);

    // get first item 
    hash_tab_test_t* itemFisrtInsert = NULL;
    itemFisrtInsert = malloc(sizeof(hash_tab_test_t));
    itemFisrtInsert->item.key = 10;
    itemFisrtInsert->item.next = NULL;
    itemFisrtInsert->varTest = keySize - 10;
    tab_item_t* itemObj = (tab_item_t*)itemFisrtInsert;
    assert(RE_OK == item_insert(pTab, itemObj));
    assert(itemObj == tab_item_first(pTab));
    assert(RE_OK == item_delete(pTab, 10));

    // insert 
    hash_tab_test_t* itemInsert = NULL;
    for(int i = 0; i < keySize; i ++)
    {
        itemInsert = malloc(sizeof(hash_tab_test_t));
        itemInsert->item.key = i;
        itemInsert->item.next = NULL;
        itemInsert->varTest = keySize - i;
        tab_item_t* itemObj = (tab_item_t*)itemInsert;
        assert(RE_OK == item_insert(pTab, itemObj));
    }

    // get first item and delete it 
    hash_tab_test_t* itemDelete = NULL;
    for(int i = 0; i < keySize; i ++)
    {
        itemDelete = (hash_tab_test_t*)tab_item_first(pTab);
        assert(itemDelete);
        assert(pTab->size = keySize - i);
        assert(RE_OK == item_delete(pTab,itemDelete->item.key));
    }

}

void insert_search_delete_test()
{
    // init 
    int keySize = 1024;     // 2^10
    int bucketRate = 5;     // list max item are 
    hash_table_t* pTab = tab_init(keySize, bucketRate);
    assert(pTab);
    assert(32 == pTab->bucketSize);

    // insert 
    hash_tab_test_t* itemInsert = NULL;
    for(int i = 0; i < keySize; i ++)
    {
        itemInsert = malloc(sizeof(hash_tab_test_t));
        itemInsert->item.key = i;
        itemInsert->item.next = NULL;
        itemInsert->varTest = keySize - i;
        tab_item_t* itemObj = (tab_item_t*)itemInsert;
        assert(RE_OK == item_insert(pTab, itemObj));
    }

    assert(keySize == pTab->size);

    // search
    hash_tab_test_t* itemSearch = NULL;
    for(int i = 0; i < keySize; i ++)
    {
        itemSearch = (hash_tab_test_t*)item_search(pTab, i);
        assert(itemSearch);
        assert(itemSearch->varTest == (keySize - i));
    }

    // delete 
    for(int i = 0; i < keySize; i ++)
    {
        assert(pTab->size == (keySize - i));
        assert(RE_OK == item_delete(pTab, i));
    }

    // get first item 
    hash_tab_test_t* itemFisrtInsert = NULL;
    itemFisrtInsert = malloc(sizeof(hash_tab_test_t));
    itemFisrtInsert->item.key = 10;
    itemFisrtInsert->item.next = NULL;
    itemFisrtInsert->varTest = keySize - 10;
    tab_item_t* itemObj = (tab_item_t*)itemFisrtInsert;
    assert(RE_OK == item_insert(pTab, itemObj));
    assert(itemObj == tab_item_first(pTab));
}

int main(int argc, void* argv[])
{
    printf("hash table test start!\n");
    // test case 

    // init and deinit 
    init_deinit_test();
    insert_search_delete_test();

    // insert 
    insert_test();

    // search test 
    search_test();

    // delete test 
    delete_test();

    // first item get test 
    first_item_get_test();

    // fuzz test
    insert_search_delete_test();

    printf("hash table test end!\n");
    return 0;
}
#include "phash.h"

long int generate_key(char *str, long int mod)
{
    int i;
    long int key_gen = 0;
    int len = strlen(str);

    for (i=0; i < 100 ; ++i)
            key_gen = ((key_gen * 26) + str[i]) % mod;

    return key_gen;
}

long int get_hash_key(char *string, int a, int b)
{
    long int key = generate_key(string, MOD1);
    return (a * key + b) % MOD1;
}

int combi(int n)
{
    return (n * (n-1)) / 2;
}

long int count_collisions(struct level_one* table, int size)
{
    int i;
    long int count = 0;
    for (i = 0; i < size; ++i)
        count += combi(table[i].freq);

}

void record_index(struct level_one* table, int loc, int _index)
{
    struct string_index *new_node = (struct string_index*) malloc(sizeof(struct string_index));
    new_node->index = _index;
    new_node->next = table[loc].next_index;
    table[loc].next_index = new_node;
}


struct level_two *create_second_level(int size)
{
    return (struct level_two *) calloc (size, sizeof(struct level_two));
}

long int secondary_hash(char *string, int a, int b)
{
    long int key = generate_key(string, MOD2);
    return (a * key + b) % MOD2;
}


void print_hash_table(struct level_one* table, long int *key_storage)
{
    // printing hash table
    struct level_two *secondary_ptr = NULL;
    struct string_index *str_index = NULL;
    int i,j,k,s;
    for(i = 0; i < PRIME_VAL; ++i)
    {
        if(table[i].freq != 0)
        {
            j = (table[i].freq) * (table[i].freq);
            secondary_ptr = table[i].next_level;
            str_index = table[i].next_index;
            puts("-----------------------");
            printf("slot : %d items : %d size : %d\n",i,table[i].freq,table[i].freq * table[i].freq );

            while(str_index)
            {
                printf("%ld ", key_storage[str_index->index]);
                str_index = str_index->next;
            }
            printf("\n" );
            puts("-----------------------");
            for(k = 0; k < j; ++k)
            {
                if(secondary_ptr[k].present)
                    printf(" %ld ", secondary_ptr[k].hash_value);
            }

        }
        printf("\n");
    }

}

int find_string(struct level_one *table, long long int z1, long long int z2,int ga, int gb)
{
    static emp_count = 0;
    // get the first level bucket the string would've mapped
    long int level_one_bucket = ((ga * z1 + gb) % MOD1) % PRIME_VAL;

    if(table[level_one_bucket].freq == 0)
    {
        return -1;
    }
    // get the second level bucket the string would've mapped
    // in second level check if `present` variable if it is true compare
    int a = table[level_one_bucket].a;
    int b = table[level_one_bucket].b;
    int t_size = table[level_one_bucket].freq * table[level_one_bucket].freq;

    long int level_two_hash = (a * z2 + b) % MOD2;
    long int level_two_bucket = level_two_hash % (t_size);
    struct level_two *secondary_table = table[level_one_bucket].next_level;

    if(secondary_table[level_two_bucket].hash_value == level_two_hash)
    {
        return secondary_table[level_two_bucket].index;
    }
    return -1;
}

int compare_substring(char *first, char *mstring, int i, int j)
{

    char *a, *b;
    a = first;
    b = &mstring[i];

    for( ; *a != '\0'; ++a, ++b)
    {
        if(*a != *b)
            return 1;
    }
    return 0;
}

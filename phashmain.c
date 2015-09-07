#include "phash.h"


int main(void)
{
    int i,j,k;
    int count = 0;
    int secondary_loc;
    long int secondary_hash_value;
    int string_set_index;


    long long int Z1, Z2;

    struct string_index *index_ptr = NULL;
    struct level_two *secondary_ptr = NULL;
    struct string_index* new_node = NULL;

    unsigned int collision_flag, collision_count;
    unsigned int seed = time(NULL);
    srand(seed);

    int a = 1 + (rand() % (PRIME_VAL - 1));
    int b = rand() % PRIME_VAL;

    char input[201];
    char *main_string = (char *)malloc(100000 * sizeof(char));

    long int *key_storage = (long int *) calloc(10000, sizeof(long int));

    char** string_set = (char **)calloc (10000, sizeof(char *));
    for(i = 0 ; i < 10000; ++i)
        string_set[i] = (char *) malloc(101 * sizeof(char));


    struct level_one *primary_table = (struct level_one*)calloc(PRIME_VAL, sizeof(struct level_one));

    scanf("%s", main_string);

    for(i = 0; i < 10000; ++i)
    {
        scanf("%s", string_set[i]);
        key_storage[i] = generate_key(string_set[i], MOD1) ;
        k = get_hash_key(string_set[i],a , b) % PRIME_VAL;
        ++(primary_table[k].freq);

        record_index(primary_table,k,i);
    }


    /* BUILDING SECOND LEVEL HASH-TABLE */
    for(i = 0; i < PRIME_VAL; ++i)
    {
        if(primary_table[i].freq)
        {
            primary_table[i].next_level = create_second_level(primary_table[i].freq * primary_table[i].freq);
        }
    }

    /* GENERATING HASH FUNCTION FOR THE SECONDARY HASH FUNCTION AND PERFORMING HASHING */
    for(i = 0; i < PRIME_VAL; ++i)
    {
        collision_flag = 0;
        collision_count = 0;
        if(primary_table[i].freq)
        {
            do{
                if(collision_count > MAX_COLLISION)
                {
                    printf("%s\n", "UNABLE TO BUILD HASH TABLE FOR THE DATASET");
                    exit(1);
                }

                primary_table[i].a = 1 + (rand() % (PRIME_VAL - 1));
                primary_table[i].b = rand() % PRIME_VAL;

                secondary_ptr = primary_table[i].next_level;
                index_ptr = primary_table[i].next_index;

                while(index_ptr)
                {
                    k = index_ptr->index;
                    /*
                    	 secondary_loc = secondary_hash(key_storage[k], primary_table[i].a, primary_table[i].b)
                                                                         % (primary_table[i].freq * primary_table[i].freq);
					*/

                    secondary_hash_value = secondary_hash(string_set[k], primary_table[i].a, primary_table[i].b);
                    secondary_loc  = secondary_hash_value % (primary_table[i].freq * primary_table[i].freq);

                    // MOD WITH TABLE SIZE
                    /*
                        accessing index of the key generated earlier else we can go to the
                        string location and compute a new key using a new prime value
                        for the secondary hashing
                    */
                    collision_flag = 0;
                    if( secondary_ptr[secondary_loc].present == 0) // if no element is hashed yet control will enter the if block
                    {
                        secondary_ptr[secondary_loc].index = k; // storing array index into secondary table
                        secondary_ptr[secondary_loc].hash_value = secondary_hash_value;
                        secondary_ptr[secondary_loc].present = 1;
                    }
                    else
                    {// if collision occurs 'else' is executed

                        //puts("collision occured");

                        collision_flag = 1; // set collision flag
                        ++collision_count;

                        // reset all fields and move `index_ptr` pointer to the begining
                        free(primary_table[i].next_level);
                        primary_table[i].next_level = create_second_level(primary_table[i].freq * primary_table[i].freq );
                        index_ptr = primary_table[i].next_index;

                        break;
                    }

                    index_ptr = index_ptr->next;
                }

            }	while(collision_flag);
        }
    }

    //puts("hash table created successfully");

    //print_hash_table(primary_table,key_storage);


    /* get Z value for first 100 elements from 0 - 99 */

    Z1 = 0; Z2 = 0;

    for(i = 0; i < 100; ++i)
    {
            Z1 = ((26 * Z1) + main_string[i]) % MOD1;
            Z2 = ((26 * Z2) + main_string[i]) % MOD2;
    }


    string_set_index = find_string(primary_table,Z1,Z2,a,b);

    if(string_set_index >= 0)
    {// perform string matching at string_set[string_set_index] with main_string[i-100, i-1]

        if(compare_substring(string_set[string_set_index], main_string, i-99, i ) == 0)
        {
            ++count;
            printf("match at index : %d\n", string_set_index);
        }
    }

    for(i = 100; i < 100000; ++i)
    {

        Z1 = (Z1 + MOD1 - (RM1 * main_string[i-100]) % MOD1) % MOD1;
        Z1 = (Z1*26 + main_string[i]) % MOD1;

        // Z1 = Z1 - main_string[i - 100];
		// Z1 = (Z1 * INV_MOD1) % MOD1;
		// Z1 = (Z1 + (RM1 * main_string[i]) % MOD1) % MOD1;
        //
        //
        // Z2 = Z2 - main_string[i - 100];
		// Z2 = (Z2 * INV_MOD2) % MOD2;
		// Z2 = (Z2 + (RM2 * main_string[i]) % MOD2) % MOD2;

        Z2 = (Z2 + MOD2 - (RM2 * main_string[i-100]) % MOD2) % MOD2;
        Z2 = (Z2*26 + main_string[i]) % MOD2;

        string_set_index = find_string(primary_table,Z1,Z2,a,b);

        if(string_set_index >= 0)
        {// perform string matching at string_set[string_set_index] with main_string[i-100, i-1]
            if(compare_substring(string_set[string_set_index], main_string, i-99, i ) == 0)
            {
                ++count;
                printf("match at index : %d\n", string_set_index);
            }
        }

    }

    printf("count matching : %d\n", count);

    return EXIT_SUCCESS;
}

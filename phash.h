#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#ifndef _PHASH_H
#define _PHASH_H

#define PRIMARY_TABLE_SIZE 20011
#define MOD1 10011101
#define INV_MOD1 8855974
#define RM1 9759320

#define MOD2 10010101
#define INV_MOD2 3465035
#define RM2 7052005

#define MAX_COLLISION 100

struct string_index
{
    int index;
    struct string_index *next;
};

struct level_one
{
    int freq;   // holds the total items hashed into the current slot
    int a, b;   // hash function constants: ( (a) * key + (b) ) % MOD
    struct level_two *next_level;
    struct string_index *next_index;
};

struct level_two
{
    int index;
    long int hash_value; // secondary hash value
    int present;
};

long int generate_key(char *str, long int mod);
/* generates key from a string takes input a string pointer and a large prime number */

struct level_two *create_second_level(int );
/* allocates memory for second level hash table for a particular bucket */

long int get_hash_key(char *string, int a, int b);
/*
	returns the value after applyting hash function on the integer key of a string,
	if we perform modulus with the primary table size then we'll get the location
	to which we've to hash the element.
*/

void record_index(struct level_one *table, int table_index,int string_index);
/*
	it stores the index of the string in the linked list of a particular
	slot in primary hash table if it is mapped to that location
*/


long int count_collisions(struct level_one* table, int size);
/* counts total number of collision in first level */


long int secondary_hash(char *string, int a, int b);
/*
	 returns the value after applying hash function of the secondary table
	 if we mod it with the secondary table size we'll find the location in secondary table to insert it.
*/

void print_hash_table(struct level_one* table);
/*
	it prints the whole hash table
*/

int find_string(struct level_one *table, long long int z1, long long int z2,int ga, int gb);
/*
	returns the index of the string from the hash table by mapping the input stirng
	in to hash table and checking if we've any hash value match at that position

	table --> pointer to the primary hash table
	z1 --> key value for the primary table
	z2 --> key value for the secondary table
	ga & gb --> parameters of the hash function (a * (key) + b ) used in the particular secondary table
*/

int compare_substring(char *first, char *mstring, int mstring_start, int mstring_end);
/*
	compares two strings returns 0 if both are equal else returns 1

	char *first --> points to a string of fixed length 100 chars.
	mstring --> points to the large string in which we're searching and in the range
				starting from 'mstring_start' to 'mstring_end'.
*/

#endif

/*
 * A339479  Number of partitions consisting of n parts, each of which is a
 *          power of 2, where one part is 1 and no part is more than twice the
 *          sum of all smaller parts.
 * https://oeis.org/A339479
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 45, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A339479 implementation
   The sequence is defined by the recursion
      F(0,k) = 1
      F(n,0) = F(n-1,1)          for n>0
      F(n,k) = F(n-1,k+1) + F(n, floor(k/2))  for n,k>0
   and a(n) = F(n,0).  The implementation memoises all computed
   values in a hash table (C) or unordered_map (C++).  The hash
   table uses a fixed power‑of‑two size to keep look‑ups fast.
*/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef __cplusplus
/* Simple open‑addressing hash map for the C compiler (seq_t is an unsigned integer). */
typedef struct {
    uint64_t key;   /* 0 means empty (n>0 and k>0 ensure key!=0). */
    seq_t    val;
} Node;

#define INITIAL_TABLE_SIZE (1 << 20)   /* 1,048,576 entries; power of two for fast modulo. */
static const size_t hsize = INITIAL_TABLE_SIZE;
static const size_t hmask = hsize - 1;
static Node *htable = NULL;

static void init_hash(void)
{
    if (!htable)
        htable = (Node*)calloc(hsize, sizeof(Node));
}

/* SplitMix64 hash function */
static uint64_t hash_key(uint64_t k)
{
    k += 0x9e3779b97f4a7c15ULL;
    k = (k ^ (k >> 30)) * 0xbf58476d1ce4e5b9ULL;
    k = (k ^ (k >> 27)) * 0x94d049bb133111ebULL;
    return k ^ (k >> 31);
}

static seq_t *hash_find(uint64_t key)
{
    init_hash();
    size_t idx = hash_key(key) & hmask;
    for (;;)
    {
        if (!htable[idx].key) return NULL;            /* empty slot */
        if (htable[idx].key == key) return &htable[idx].val;
        idx = (idx + 1) & hmask;                     /* linear probing */
    }
}

static void hash_insert(uint64_t key, seq_t val)
{
    init_hash();
    size_t idx = hash_key(key) & hmask;
    for (;;)
    {
        if (!htable[idx].key) {                      /* empty slot found */
            htable[idx].key = key;
            htable[idx].val = val;
            return;
        }
        if (htable[idx].key == key) {                /* overwrite existing key */
            htable[idx].val = val;
            return;
        }
        idx = (idx + 1) & hmask;
    }
}
#else
/* C++ compiler uses unordered_map for simplicity */
#include <unordered_map>
static std::unordered_map<uint64_t, seq_t> memo;
#endif

/* Recursive function implementing the recurrence */
static seq_t F(idx_t n, idx_t k)
{
    if (n == 0) return (seq_t)1;     /* base case */

    /* encode pair (n,k) as 64‑bit key */
    uint64_t key = ((uint64_t)n << 32) | (uint64_t)k;

#ifdef __cplusplus
    auto it = memo.find(key);
    if (it != memo.end())
        return it->second;
#else
    seq_t *ptr = hash_find(key);
    if (ptr)
        return *ptr;
#endif

    seq_t result;
    if (k == 0)
        result = F(n - 1, 1);
    else
        result = F(n - 1, k + 1) + F(n, k / 2);

#ifdef __cplusplus
    memo[key] = result;
#else
    hash_insert(key, result);
#endif

    return result;
}

/* External linkage as requested */
seq_t a(idx_t n)
{
    return F(n, 0);
}

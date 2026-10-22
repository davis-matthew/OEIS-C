/*
 * A345605  Numbers that are the sum of seven fifth powers in two or more
 *          ways.
 * https://oeis.org/A345605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A345605: Numbers that are the sum of seven fifth powers in two or more ways.
   Implementation uses a hash table to count occurrences of each sum of seven
   fifth powers (with repetition allowed, ordered tuples counted as the same
   because we generate sums for non‑decreasing 7‑tuples).  All sums that occur
   at least twice are stored in a sorted cache array.  The function
   seq_t a(idx_t n) returns the n‑th term of the sequence (offset 1).  */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* --------------------------------------------------------------------------
   Static state
   -------------------------------------------------------------------------- */
static uint32_t *hash_keys = NULL;
static uint8_t *hash_count = NULL;
static size_t  hash_cap = 0;
static size_t  hash_mask = 0;

static seq_t *cache = NULL;
static size_t cache_len = 0;
static bool cache_built = false;

/* --------------------------------------------------------------------------
   Utility: comparator for qsort
   -------------------------------------------------------------------------- */
static int cmp_seq_t(const void *a, const void *b)
{
    seq_t x = *(const seq_t *)a;
    seq_t y = *(const seq_t *)b;
    return (x > y) - (x < y);
}

/* --------------------------------------------------------------------------
   Insert a sum into the hash table (open addressing, linear probing).
   -------------------------------------------------------------------------- */
static inline void insert_sum(uint32_t sum)
{
    size_t idx = ((size_t)sum * 2654435761u) & hash_mask;
    while (true) {
        uint32_t k = hash_keys[idx];
        if (k == 0) {                     /* empty slot */
            hash_keys[idx] = sum;
            hash_count[idx] = 1;
            return;
        }
        if (k == sum) {                   /* key already there */
            hash_count[idx] += 1;
            return;
        }
        idx = (idx + 1) & hash_mask;      /* probe */
    }
}

/* --------------------------------------------------------------------------
   Build the cache of all sums that have at least two representations.
   -------------------------------------------------------------------------- */
static void build_cache(void)
{
    if (cache_built)
        return;

    /* Parameters -------------------------------------------------------- */
    const int MAX_BASE = 35;          /* enough to obtain >10 000 terms */
    const size_t COMB_N = 22500000;   /* approximate number of 7‑tuples */

    /* Compute 5th powers for 1..MAX_BASE into a 32‑bit array            */
    uint32_t power[MAX_BASE + 1];
    power[0] = 0;
    for (int i = 1; i <= MAX_BASE; ++i) {
        unsigned long long val = 1;
        for (int k = 0; k < 5; ++k)
            val *= (unsigned long long)i;
        power[i] = (uint32_t)val;      /* 35^5 < 2^32 */
    }

    /* Allocate hash table --------------------------------------------- */
    hash_cap = 1u << 25;              /* 33 554 432 slots, load < 0.7 */
    hash_mask = hash_cap - 1;
    hash_keys  = (uint32_t *)calloc(hash_cap, sizeof(uint32_t));
    hash_count = (uint8_t  *)calloc(hash_cap, sizeof(uint8_t));

    /* Enumerate all non‑decreasing 7‑tuples (a..g) - 22.5 M tuples */
    for (int a = 1; a <= MAX_BASE; ++a) {
        uint32_t pa = power[a];
        for (int b = a; b <= MAX_BASE; ++b) {
            uint32_t s2 = pa + power[b];
            for (int c = b; c <= MAX_BASE; ++c) {
                uint32_t s3 = s2 + power[c];
                for (int d = c; d <= MAX_BASE; ++d) {
                    uint32_t s4 = s3 + power[d];
                    for (int e = d; e <= MAX_BASE; ++e) {
                        uint32_t s5 = s4 + power[e];
                        for (int f = e; f <= MAX_BASE; ++f) {
                            uint32_t s6 = s5 + power[f];
                            for (int g = f; g <= MAX_BASE; ++g) {
                                uint32_t total = s6 + power[g];
                                insert_sum(total);
                            }
                        }
                    }
                }
            }
        }
    }

    /* Count how many sums appear at least twice ------------------------ */
    size_t dup_cnt = 0;
    for (size_t i = 0; i < hash_cap; ++i)
        if (hash_keys[i] && hash_count[i] >= 2)
            dup_cnt++;

    /* Allocate cache array --------------------------------------------- */
    cache = (seq_t *)malloc(dup_cnt * sizeof(seq_t));
    cache_len = dup_cnt;

    /* Extract sums with count >= 2 ------------------------------------- */
    size_t pos = 0;
    for (size_t i = 0; i < hash_cap; ++i) {
        if (hash_keys[i] && hash_count[i] >= 2)
            cache[pos++] = (seq_t)hash_keys[i];
    }

    /* Sort cache so that a(n) is the n‑th smallest value ---------------- */
    qsort(cache, cache_len, sizeof(seq_t), cmp_seq_t);

    cache_built = true;
}

/* --------------------------------------------------------------------------
   Public function required by the task
   -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for offset < 1 */
    build_cache();
    if ((size_t)n > cache_len)
        return 0;                     /* out of computed range */
    return cache[(size_t)n - 1];
}

/*
 * A390319  Numbers k with exactly five coincident values among the fractions
 *          (2*k mod m)/m for 1 <= m <= k.
 * https://oeis.org/A390319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A390319:
   Numbers k with exactly five coincident values among the fractions
   (2*k mod m)/m for 1 <= m <= k.
   This implementation performs a linear search for k, keeps found
   terms in a static array, and continues searching until the
   requested index has been reached. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd_uint64.h"

/* ----- hash table to count reduced fractions -------------------------------- */
#define HASH_BITS   18               /* 2^18 = 262144 slots */
#define HASH_SIZE   (1U << HASH_BITS)
#define HASH_MASK   (HASH_SIZE - 1U)

typedef struct {
    uint64_t key;   /* combined numerator/denominator */
    uint32_t count; /* how many times the key appeared */
    uint32_t gen;   /* generation when the entry was last used */
} HashEntry;

static HashEntry hash_table[HASH_SIZE];
static uint32_t  used_indices[HASH_SIZE];
static uint32_t  used_len   = 0;
static uint32_t  current_gen = 0;

static inline uint32_t hash_put(uint64_t key)
{
    size_t h = (size_t)((key * 11400714819323198485ULL)) & HASH_MASK;
    while (1) {
        if (hash_table[h].gen != current_gen) {   /* empty slot in this gen */
            hash_table[h].gen   = current_gen;
            hash_table[h].key   = key;
            hash_table[h].count = 1;
            used_indices[used_len++] = (uint32_t)h;
            return 1;
        }
        if (hash_table[h].key == key) {           /* key already present */
            hash_table[h].count++;
            return hash_table[h].count;
        }
        h = (h + 1) & HASH_MASK;                  /* linear probing */
    }
}

/* ----- dynamic array for found terms -------------------------------------- */
static seq_t *found_terms = NULL;
static size_t found_cap   = 0;
static size_t found_len   = 0;

static void add_term(seq_t val)
{
    if (found_len == found_cap) {
        size_t new_cap = (found_cap == 0) ? 16 : found_cap << 1;
        seq_t *tmp = (seq_t*)realloc(found_terms, new_cap * sizeof(seq_t));
        if (!tmp) abort();               /* allocation failed */
        found_terms = tmp;
        found_cap   = new_cap;
    }
    found_terms[found_len++] = val;
}

/* ----- limits and state variables ----------------------------------------- */
static const uint64_t LIMIT = 2000000ULL;   /* enough for all known terms */
static uint64_t next_k = 1;                /* next k to examine */

/* ----- main function required by the competition --------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* out of range */

    /* continue searching until we have at least n terms or run out of k */
    while (found_len < (size_t)n && next_k <= LIMIT) {
        uint64_t k = next_k++;
        ++current_gen;           /* new generation for this k */
        used_len = 0;

        /* count distinct reduced fractions with multiplicity */
        for (uint64_t m = 1; m <= k; ++m) {
            uint64_t numer = (2 * k) % m;   /* (2k mod m) */
            if (numer == 0) {
                hash_put(0);                 /* 0/1 */
            } else {
                uint64_t g = gcd_uint64(numer, m);
                numer /= g;
                uint64_t denom = m / g;
                uint64_t key = (numer << 32) | (denom & 0xffffffffULL);
                hash_put(key);
            }
        }

        /* count how many distinct reduced fractions appear at least twice */
        uint32_t repeated = 0;
        for (uint32_t i = 0; i < used_len; ++i) {
            const HashEntry *e = &hash_table[used_indices[i]];
            if (e->count > 1) ++repeated;
        }
        if (repeated == 5) {
            add_term((seq_t)k);
        }
    }

    if ((size_t)n > found_len) return (seq_t)0; /* still not found */
    return found_terms[n-1];
}

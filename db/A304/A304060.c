/*
 * A304060  Number of nX3 0..1 arrays with every element unequal to 2, 3, 4 or
 *          5 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A304060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A304060:
 *  Recurrence (valid for n > 16):
 *      a(n) = 3a(n-1) +14a(n-2)+3a(n-3)
 *           -75a(n-4) -79a(n-5)+61a(n-6)
 *           +113a(n-7)+38a(n-8) -21a(n-9)
 *           +5a(n-10)+23a(n-11)-6a(n-12)
 *           -8a(n-13)+4a(n-14)
 *  Initial values:
 *      a(1)=0,  a(2)=5,  a(3)=14,  a(4)=69,  a(5)=376,
 *      a(6)=1891, a(7)=9889, a(8)=51283, a(9)=265582, a(10)=1378735,
 *      a(11)=7148318, a(12)=37076755, a(13)=192302847,
 *      a(14)=997352273, a(15)=5172817910, a(16)=26828692621
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed terms (1‑based index => 0‑based storage) */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;   /* number of terms stored */
    static size_t cache_capacity = 0;

    /* initialise cache with the first 16 known terms */
    if (cache == NULL) {
        cache_capacity = 64;       /* start with room for many terms */
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (cache == NULL) return ((seq_t)0); /* out of memory – should not happen */

        cache[0]  = (seq_t)0ULL;
        cache[1]  = (seq_t)5ULL;
        cache[2]  = (seq_t)14ULL;
        cache[3]  = (seq_t)69ULL;
        cache[4]  = (seq_t)376ULL;
        cache[5]  = (seq_t)1891ULL;
        cache[6]  = (seq_t)9889ULL;
        cache[7]  = (seq_t)51283ULL;
        cache[8]  = (seq_t)265582ULL;
        cache[9]  = (seq_t)1378735ULL;
        cache[10] = (seq_t)7148318ULL;
        cache[11] = (seq_t)37076755ULL;
        cache[12] = (seq_t)192302847ULL;
        cache[13] = (seq_t)997352273ULL;
        cache[14] = (seq_t)5172817910ULL;
        cache[15] = (seq_t)26828692621ULL;
        cache_size = 16;
    }

    /* if the requested term is already known, return it */
    if (n <= cache_size && n >= 1) return cache[n-1];

    /* otherwise compute all missing terms up to n */
    for (idx_t j = cache_size + 1; j <= n; ++j) {
        seq_t next = 0;
        next += (seq_t)3ULL * cache[j-2];
        next += (seq_t)14ULL * cache[j-3];
        next += (seq_t)3ULL * cache[j-4];
        next -= (seq_t)75ULL * cache[j-5];
        next -= (seq_t)79ULL * cache[j-6];
        next += (seq_t)61ULL * cache[j-7];
        next += (seq_t)113ULL * cache[j-8];
        next += (seq_t)38ULL * cache[j-9];
        next -= (seq_t)21ULL * cache[j-10];
        next += (seq_t)5ULL * cache[j-11];
        next += (seq_t)23ULL * cache[j-12];
        next -= (seq_t)6ULL * cache[j-13];
        next -= (seq_t)8ULL * cache[j-14];
        next += (seq_t)4ULL * cache[j-15];

        /* extend the cache if necessary */
        if (cache_size == cache_capacity) {
            size_t new_cap = cache_capacity * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return ((seq_t)0); /* allocation failed */
            cache = tmp;
            cache_capacity = new_cap;
        }
        cache[cache_size] = next;
        ++cache_size;
    }

    return cache[n-1];
}

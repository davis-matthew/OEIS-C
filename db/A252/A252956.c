/*
 * A252956  Number of (n+2) X (3+2) 0..4 arrays with every consecutive three
 *          elements in every row and diagonal having exactly two distinct
 *          values, and in every column and antidiagonal not having exactly
 *          two distinct values, and new values 0 upwards introduced in row
 *          major order.
 * https://oeis.org/A252956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=223, a(2)=155, a(3)=259, a(4)=443, a(5)=871, a(6)=1681,
   a(7)=2999, a(8)=6103, a(9)=11983, a(10)=21749.
   For n>10: a(n)=4*a(n-1)-2*a(n-2)-3*a(n-3)-13*a(n-4)
             +8*a(n-5)+70*a(n-6)-105*a(n-7)+42*a(n-8)   */
/* The function memoizes results so calls in any order remain O(n). */
#include <stdlib.h>
#include <stdint.h>

/* If seq_t is a built‑in unsigned type (uint8_t…uint128_t) the
   operations below work as usual. For an arbitrary‑precision type
   the lib handles +, -, * as needed. */

seq_t a(idx_t n)
{
    /* 1‑indexing in the cache; index 0 unused. */
    static seq_t *cache = NULL;          /* array of cached values   */
    static size_t cache_cap = 0;         /* capacity of cache        */
    static idx_t  computed_max = 0;      /* largest n computed so far*/
    idx_t i;

    /* Capacity growth helper */
    if (cache_cap < (size_t)n + 1) {
        size_t new_cap = (cache_cap == 0) ? 64 : (cache_cap << 1);
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
        /* initialise the new part with zero (default). */
        for (size_t j = computed_max + 1; j < cache_cap; ++j) cache[j] = 0;
    }

    /* Initialise the first ten terms on the first call. */
    if (computed_max == 0) {
        computed_max = 10;
        cache[1]  = 223;
        cache[2]  = 155;
        cache[3]  = 259;
        cache[4]  = 443;
        cache[5]  = 871;
        cache[6]  = 1681;
        cache[7]  = 2999;
        cache[8]  = 6103;
        cache[9]  = 11983;
        cache[10] = 21749;
    }

    if (n <= computed_max) return cache[n];

    /* Compute all missing terms up to n. */
    for (i = computed_max + 1; i <= n; ++i) {
        seq_t pos = 0, neg = 0;
        pos += (seq_t)4 * cache[i-1];
        pos += (seq_t)8 * cache[i-5];
        pos += (seq_t)70 * cache[i-6];
        pos += (seq_t)42 * cache[i-8];

        neg += (seq_t)2 * cache[i-2];
        neg += (seq_t)3 * cache[i-3];
        neg += (seq_t)13 * cache[i-4];
        neg += (seq_t)105 * cache[i-7];

        cache[i] = pos - neg;  /* pos >= neg for all known terms */
    }

    computed_max = n;
    return cache[n];
}

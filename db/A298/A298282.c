/*
 * A298282  Number of n X 3 0..1 arrays with every element equal to 0, 1, 2, 3
 *          or 6 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A298282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A298282: Number of n×3 0/1 arrays where each entry equals the number of
 * king‑move adjacent 1s (allowed values 0,1,2,3,6), upper left cell = 0.
 *
 * Empirical recurrence for n>12:
 *   a(n) = 3*a(n-1) - a(n-2) + 2*a(n-3) + 4*a(n-4) - a(n-5)
 *          + 2*a(n-6) - 2*a(n-7) - 3*a(n-8) - 2*a(n-9) - 2*a(n-10) - 2*a(n-11)
 *
 * Initial terms (n=1..12):
 *   a(1)=4,  a(2)=25,  a(3)=70,  a(4)=205,  a(5)=614,  a(6)=1860,
 *   a(7)=5631, a(8)=17034, a(9)=51507, a(10)=155755, a(11)=471038, a(12)=1424553
 *
 * The function below implements this recurrence with memoisation and works for any
 * n≥1, in O(n) time and O(n) space.
 */

#include <stdlib.h>
#include <stdint.h>

/* External types provided by the build environment */
typedef uint64_t idx_t;

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* undefined for n=0, return 0 as sentinel */

    /* static cache: cache[i] holds a(i) for 1 ≤ i < cache_cap   */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;    /* number of elements allocated (indices 0..cache_cap-1) */
    static size_t computed = 0;     /* highest n for which a(n) is already computed */

    /* Initial values up to n=12 */
    static const seq_t seeds[13] = {0ULL,
        4ULL, 25ULL, 70ULL, 205ULL, 614ULL, 1860ULL,
        5631ULL, 17034ULL, 51507ULL,
        155755ULL, 471038ULL, 1424553ULL};

    /* Ensure that the cache can hold indices up to n */
    size_t need = (size_t)n + 1;    /* because indices start at 1 (0 unused) */
    if (need > cache_cap) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        cache = tmp;
        cache_cap = need;
        /* initialise seeds (safe to overwrite existing values) */
        size_t up_to = need <= 13 ? need : 13;
        for (size_t i = 1; i < up_to; ++i) cache[i] = seeds[i];
    }

    /* After first allocation initialise 'computed' */
    if (computed == 0) {
        computed = (cache_cap > 12 ? 12 : cache_cap - 1);
    }

    /* Return if already computed */
    if (n <= computed) return cache[(size_t)n];

    /* Compute missing terms using recurrence */
    for (size_t k = (computed >= 13 ? computed + 1 : 13); k <= (size_t)n; ++k) {
        seq_t sum_pos = 0;
        seq_t sum_neg = 0;

        /* positive contributions */
        sum_pos += 3 * cache[k-1];
        sum_pos += 2 * cache[k-3];
        sum_pos += 4 * cache[k-4];
        sum_pos += 2 * cache[k-6];

        /* negative contributions */
        sum_neg += 1 * cache[k-2];
        sum_neg += 1 * cache[k-5];
        sum_neg += 2 * cache[k-7];
        sum_neg += 3 * cache[k-8];
        sum_neg += 2 * cache[k-9];
        sum_neg += 2 * cache[k-10];
        sum_neg += 2 * cache[k-11];

        cache[k] = sum_pos - sum_neg;  /* sum_pos > sum_neg by recurrence definition */
    }

    computed = (size_t)n;
    return cache[(size_t)n];
}

/*
 * A266062  Number of nX2 integer arrays with each element equal to the number
 *          of horizontal, vertical, diagonal and antidiagonal neighbors
 *          exactly one smaller than itself.
 * https://oeis.org/A266062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)
 *            +2*a(n-2)+20*a(n-3)+30*a(n-4)-50*a(n-5)-160*a(n-6)-512*a(n-7)-404*a(n-8)
 *            +948*a(n-9)+1880*a(n-10)+4248*a(n-11)+1088*a(n-12)-3776*a(n-13)-7488*a(n-14)
 *            -5376*a(n-15)+4352*a(n-16)+4608*a(n-17)+8192*a(n-18) (n>18)
 *            initial values: a(1)=3, a(2)=17, a(3)=35, a(4)=89, a(5)=323,
 *            a(6)=1057, a...
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* number of slots currently allocated (>= n+1) */

static void ensure_cache(idx_t n)
{
    if ((size_t)n < cache_cap)
        return;                     /* already have slots */

    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    if (new_cap < (size_t)n + 1)
        new_cap = (size_t)n + 1;     /* need index n, so at least n+1 slots */

    /* allocate new array, copy existing data, free old array */
#ifdef __cplusplus
    seq_t *new_cache = new seq_t[new_cap];
    if (cache) {
        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        delete[] cache;
    }
#else
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();          /* memory allocation failed */
    /* no need to copy: realloc preserves existing content */
#endif
    cache = new_cache;

    /* initialize newly allocated slots */
    /* initial values up to a(18) */
    static const seq_t init_vals[19] =
        {0ULL, 3ULL, 17ULL, 35ULL, 89ULL, 323ULL, 1057ULL, 3027ULL,
         9257ULL, 29835ULL, 92561ULL, 288027ULL, 898873ULL,
         2818275ULL, 8824849ULL, 27652507ULL, 86466761ULL,
         270984643ULL, 849833089ULL};

    for (size_t i = cache_cap; i < new_cap; ++i)
    {
        if (i <= 18) {
            cache[i] = init_vals[i];
        } else {
            seq_t val = 0;
            val += cache[i-1];
            val += (seq_t)2 * cache[i-2];
            val += (seq_t)20 * cache[i-3];
            val += (seq_t)30 * cache[i-4];
            val -= (seq_t)50 * cache[i-5];
            val -= (seq_t)160 * cache[i-6];
            val -= (seq_t)512 * cache[i-7];
            val -= (seq_t)404 * cache[i-8];
            val += (seq_t)948 * cache[i-9];
            val += (seq_t)1880 * cache[i-10];
            val += (seq_t)4248 * cache[i-11];
            val += (seq_t)1088 * cache[i-12];
            val -= (seq_t)3776 * cache[i-13];
            val -= (seq_t)7488 * cache[i-14];
            val -= (seq_t)5376 * cache[i-15];
            val += (seq_t)4352 * cache[i-16];
            val += (seq_t)4608 * cache[i-17];
            val += (seq_t)8192 * cache[i-18];
            cache[i] = val;
        }
    }

    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for offset 1, but harmless */

    ensure_cache(n);
    return cache[n];
}

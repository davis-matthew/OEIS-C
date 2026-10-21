/*
 * A297638  Number of 2Xn 0..1 arrays with every 1 horizontally, diagonally or
 *          antidiagonally adjacent to 0, 2 or 3 neighboring 1s.
 * https://oeis.org/A297638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + a(n-2) - a(n-3) + 6*a(n-4) - 28*a(n-5) -
 *            24*a(n-6) a(1)=4, a(2)=8, a(3)=29, a(4)=69, a(5)=200, a(6)=552
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Cache for already computed values */
static idx_t max_computed = 0;      /* largest n already stored in cache */
static seq_t *cache = NULL;         /* array of values, indexed from 1 */
static size_t cache_cap = 0;        /* current allocated capacity (index+1) */

/* Ensure that the cache contains known initial values */
static void init_cache(void)
{
    if (cache != NULL)
        return;

    /* Start with a reasonable capacity; will grow as needed */
    cache_cap = 64;
    cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
    if (cache == NULL)          /* allocation failed – abort program */
        abort();

    /* Initial values from the sequence definition */
    cache[1] = 4;
    cache[2] = 8;
    cache[3] = 29;
    cache[4] = 69;
    cache[5] = 200;
    cache[6] = 552;
    max_computed = 6;
}

/* Extend the cache to at least size 'needed' (neededn+1 elements) */
static void extend_cache(idx_t needed)
{
    if (needed + 1 <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap <= (size_t)(needed + 1))
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL)
        abort();                /* out‑of‑memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)            /* out of range – define as 0 */
        return 0;

    init_cache();

    /* Compute any missing terms up to n */
    if (n > max_computed) {
        extend_cache(n);
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            /* Positive part: 3*a(i-1) + a(i-2) + 6*a(i-4) */
            seq_t pos = 3 * cache[i - 1] + cache[i - 2] + 6 * cache[i - 4];
            /* Negative part: a(i-3) + 28*a(i-5) + 24*a(i-6) */
            seq_t neg = cache[i - 3] + 28 * cache[i - 5] + 24 * cache[i - 6];
            cache[i] = pos - neg;          /* unsigned subtraction is safe
                                                because pos ≥ neg for all i */
        }
        max_computed = n;
    }
    return cache[n];
}

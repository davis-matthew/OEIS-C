/*
 * A334669  Denominator of Sum_{k=1..n} (-1)^(k+1)/k^7.
 * https://oeis.org/A334669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of the alternating sum S(n) = Σ_{k=1..n}
 *            (-1)^{k+1}/k^7 returned in lowest terms. The first few values
 *            are a(1)=1, a(2)=128, a(3)=279936, …
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Helper for allocating/releasing seq_t arrays in C or C++ */
#if defined(__cplusplus)
    #define ALLOC_SEQ_T new
    #define FREE_SEQ_T delete
#else
    #define ALLOC_SEQ_T malloc
    #define FREE_SEQ_T free
#endif

/* Global cache of denominators */
static seq_t *den_cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_cap  = 0;

/* Running sum of the series */
static seq_t cur_num = (seq_t)1;
static seq_t cur_den = (seq_t)1;

/* (seq_t) Euclidean GCD */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Grow the cache array to at least 'needed' elements */
static void grow_cache(idx_t needed)
{
    if (cache_cap >= needed)
        return;

    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *new_arr;
#if defined(__cplusplus)
    new_arr = new seq_t[new_cap];
#else
    new_arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif
    if (!new_arr)
        return;          /* allocation failed – subsequent a() calls may crash */

    for (idx_t i = 0; i < cache_size; ++i)
        new_arr[i] = den_cache[i];

#if defined(__cplusplus)
    delete[] den_cache;
#else
    free(den_cache);
#endif
    den_cache = new_arr;
    cache_cap = new_cap;
}

/* Extend cache to contain denominators up to n (inclusive) */
static void extend_cache(idx_t n)
{
    if (cache_size == 0) {
        grow_cache(1);
        /* S(1) = 1/1 */
        den_cache[0] = (seq_t)1;
        cache_size  = 1;
        cur_num     = (seq_t)1;
        cur_den     = (seq_t)1;
    }

    while (cache_size < n) {
        idx_t k = cache_size + 1;          /* next term index (1‑based) */

        /* compute k^7 */
        seq_t k_pow = (seq_t)1;
        for (int i = 0; i < 7; ++i)
            k_pow *= (seq_t)k;

        /* add or subtract 1/k^7 */
        if ((k & 1) == 0)   /* even k → negative term */
            cur_num = cur_num * k_pow - cur_den;
        else                /* odd k → positive term */
            cur_num = cur_num * k_pow + cur_den;

        cur_den *= k_pow;                /* common denominator */

        /* reduce the fraction */
        seq_t g = seq_gcd(cur_num, cur_den);
        cur_num /= g;
        cur_den /= g;

        grow_cache(cache_size + 1);
        den_cache[cache_size++] = cur_den;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined, but safe */

    extend_cache(n);
    return den_cache[n - 1];
}

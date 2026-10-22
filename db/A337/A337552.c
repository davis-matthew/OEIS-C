/*
 * A337552  a(0) = 1; a(n) = Sum_{k=1..n} binomial(n,k) * (3*k-2) * a(n-k).
 * https://oeis.org/A337552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(n,k) * (3*k-2) * a(n-k), a(0)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;    /* array of computed a(n) values          */
static idx_t  cache_cap = 0;   /* capacity of the array (in seq_t elems)*/
static idx_t  cache_size = 0;  /* largest n that has been computed      */

/* Ensure that cache has space for indices 0..n inclusive and that
   a(0) is initialized.  No value beyond cache_size is initialized. */
static void ensure_cache(idx_t n)
{
    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();          /* allocation failure */
        cache = new_cache;
        cache_cap = new_cap;
    }
    if (cache_size == 0 && n >= 0) {
        /* First time: set a(0)=1 */
        cache[0] = (seq_t)1;
        cache_size = 0;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 0) return (seq_t)0;      /* undefined for negative indices */

    ensure_cache(n);

    if (cache_size < n) {
        /* compute all terms from cache_size+1 up to n */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t sum = 0;
            seq_t binom = 1;                 /* binomial(i,0) */
            for (idx_t k = 1; k <= i; ++k) {
                /* update binom to binomial(i,k) */
                binom = binom * (seq_t)(i - k + 1) / (seq_t)k;
                /* term = binom * a(i-k) * (3*k-2) */
                seq_t term = binom * cache[i - k];
                term = term * (seq_t)(3 * k - 2);
                sum += term;
            }
            cache[i] = sum;
        }
        cache_size = n;
    }

    return cache[n];
}

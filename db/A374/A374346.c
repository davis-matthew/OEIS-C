/*
 * A374346  E.g.f. A(x) satisfies A(x) = A(x^2)^(1/2) * exp(2*x) with A(0)=1.
 * https://oeis.org/A374346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A374346
   Exponential generating function:
   A(x) = exp( 2 * Σ_{k≥0} x^(2^k)/2^k )
   Recurrence derived from A'(x) = A(x) * G'(x),
   where G(x) = 2 * Σ x^(2^k)/2^k.
   The integer recurrence is
       a(0) = 1
       a(m) = Σ_{j=2^k ≤ m} 2 * Π_{t=m-j+1}^{m-1} t * a(m-j)   for m ≥ 1
   The summation runs over all powers of two j ≤ m, and the empty product
   is taken as 1.  The recurrence does not involve any divisions, so it
   can be evaluated with ordinary integer arithmetic on arbitrary‑precision
   types. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* cache for already computed a(n) values */
static seq_t *cache = NULL;
static idx_t cache_size = 0;           /* allocated size */
static idx_t cache_last = 0;           /* number of values stored */

/* per–power-of‑two production products
   prod[r] stores Π_{t=m-2^r+1}^{m-1} t for the current m in the loop.  */
static seq_t prod[64];
static int prod_initialized = 0;

/* factorial of a small integer (used only when a new power of two is hit) */
static seq_t factorial_of(idx_t n)
{
    seq_t f = (seq_t)1;
    for (idx_t k = 1; k <= n; ++k)
        f *= (seq_t)k;
    return f;
}

seq_t a(idx_t n)
{
    /* initialise cache for a(0) */
    if (cache == NULL) {
        cache = (seq_t*)calloc(1, sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out of memory */
        cache[0] = (seq_t)1;
        cache_size = 1;
        cache_last = 1;                        /* a[0] computed */
    }

    /* extend cache if needed */
    if (cache_size <= n) {
        idx_t new_size = n + 1;
        cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* out of memory */
        cache_size = new_size;
    }

    /* if n already computed, just return it */
    if (n < cache_last)
        return cache[n];

    /* initialise product array once */
    if (!prod_initialized) {
        for (int r = 0; r < 64; ++r)
            prod[r] = (seq_t)1;
        prod_initialized = 1;
    }

    /* compute missing terms up to n */
    for (idx_t m = cache_last; m <= n; ++m) {

        /* first, update prod[r] for all powers of two j <= m */
        for (int r = 0; (idx_t)1 << r <= m; ++r) {
            idx_t j = (idx_t)1 << r;
            if (m == j) {                       /* first appearance of this j */
                prod[r] = factorial_of(j - 1);  /* (j-1)!  */
            } else { /* m > j */                  /* sliding window */
                seq_t numerator   = (seq_t)(m - 1);
                seq_t denominator = (seq_t)(m - j);
                prod[r] = prod[r] * numerator / denominator;
            }
        }

        /* now compute a[m] using the updated prod array */
        seq_t sum = (seq_t)0;
        for (int r = 0; (idx_t)1 << r <= m; ++r) {
            idx_t j = (idx_t)1 << r;
            seq_t term = prod[r];
            term *= (seq_t)2;
            term *= cache[m - j];
            sum += term;
        }
        cache[m] = sum;
    }

    cache_last = n + 1;   /* all indices up to n now stored */
    return cache[n];
}

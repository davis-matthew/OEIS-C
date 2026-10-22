/*
 * A308499  Expansion of e.g.f. log(1 + Sum_{k>=1} (k+1)*(k+2)/6 * x^k).
 * https://oeis.org/A308499
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ( (n+2)! - Sum_{k=1..n-1} binomial(n-1,k)*(k+2)!*a(n-k) )
 *            / 6 with a(0)=0, a(1)=1. Uses memoization for
 *            arbitrary‑precision seq_t
 * Range:     i8 n <= 3, i16 n <= 8, i32 n <= 12, i64 n <= 20, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* cache for a(n) values */
static seq_t *a_cache      = NULL;
static idx_t  a_cache_size = 0;

/* cache for factorials: fac[i] = i! */
static seq_t *fac_cache      = NULL;
static idx_t  fac_cache_size = 0;

/* Ensure caches are large enough and compute missing a(n) values. */
static void ensure_a_cache(idx_t n)
{
    /* Allocate or grow the a(n) cache */
    const idx_t new_a_size = n + 1;
    if (a_cache == NULL) {
        a_cache      = (seq_t *)malloc(sizeof(seq_t) * new_a_size);
        a_cache_size = 0;
    } else if (new_a_size > a_cache_size) {
        a_cache = (seq_t *)realloc(a_cache, sizeof(seq_t) * new_a_size);
    }
    idx_t old_a_size = a_cache_size;
    a_cache_size = new_a_size;

    /* Allocate or grow the factorial cache: we need up to (n+2)! */
    const idx_t new_fac_size = n + 3;      /* factorial indices 0 .. n+2 */
    if (fac_cache == NULL) {
        fac_cache      = (seq_t *)malloc(sizeof(seq_t) * new_fac_size);
        fac_cache_size = 0;
    } else if (new_fac_size > fac_cache_size) {
        fac_cache = (seq_t *)realloc(fac_cache, sizeof(seq_t) * new_fac_size);
    }
    idx_t old_fac_size = fac_cache_size;
    fac_cache_size = new_fac_size;

    /* Initialise factorial[0] = 1 if cache was just created */
    if (old_fac_size == 0) {
        fac_cache[0] = (seq_t)1;
        old_fac_size = 1;
    }

    /* Compute missing factorials */
    for (idx_t i = old_fac_size; i < new_fac_size; ++i) {
        fac_cache[i] = fac_cache[i - 1] * (seq_t)i;
    }

    /* Initialise a(0) if cache was just created */
    if (old_a_size == 0) {
        a_cache[0] = (seq_t)0;
        old_a_size = 1;
    }

    /* Compute a(m) for all m that are now within range and not yet stored */
    for (idx_t m = old_a_size; m <= n; ++m) {
        if (m == 0) {
            a_cache[0] = (seq_t)0;
            continue;
        }

        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;   /* C(m-1,0) */

        for (idx_t k = 1; k <= m - 1; ++k) {
            /* Update binom to C(m-1,k) */
            binom = binom * (seq_t)(m - 1 - (k - 1));
            binom = binom / (seq_t)k;

            seq_t term = binom * fac_cache[k + 2] * a_cache[m - k];
            sum += term;
        }

        seq_t numer = fac_cache[m + 2] - sum;
        a_cache[m]  = numer / (seq_t)6;
    }
}

/* Public function: returns a(n) for any n ≥ 0.
   Works for any seq_t size, including arbitrary‑precision. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    ensure_a_cache(n);
    return a_cache[n];
}

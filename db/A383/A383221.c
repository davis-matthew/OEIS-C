/*
 * A383221  Coefficient of x^3 in expansion of (x+2) * (x+5) * ... *
 *          (x+3*n-1).
 * https://oeis.org/A383221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^3 in ∏ₖ₌₁ⁿ (x + 3k – 1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset : 0  (a(0) = 0) */
/* This implementation keeps the lowest four coefficients of the current
   polynomial and builds the sequence iteratively.  Results are cached
   so calls in any order are handled efficiently. */

#include <stdlib.h>   /* malloc, realloc, abort */

static seq_t coeff[4] = {1, 0, 0, 0};   /* coefficients of the polynomial */
static idx_t last_n   = 0;              /* highest n processed so far   */
static idx_t cache_size = 0;           /* number of cached values      */
static seq_t *cache     = NULL;        /* array of a(n) values         */

/* Ensure cache array holds indices 0 … n.  */
static void
_ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)
        abort();                       /* out of memory            */
    cache = tmp;
    cache_size = new_size;
}

/* --------------------------------------------------------------------
   seq_t a(idx_t n)
   -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Initialise cache on first use.  */
    if (!cache) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache)
            abort();                   /* out of memory            */
        cache[0] = 0;                  /* a(0) = 0                 */
        cache_size = 1;
    }

    /* Return cached value if already known. */
    if (n < cache_size)
        return cache[n];

    /* Ensure we have room in the cache. */
    _ensure_cache(n);

    /* Extend the polynomial and cache up to n. */
    for (idx_t m = last_n + 1; m <= n; ++m) {
        /* d = 3*m – 1 is the constant term of the new factor (x + d). */
        seq_t d = (seq_t)(3 * m - 1);

        /* Old coefficients before the update. */
        seq_t old0 = coeff[0];
        seq_t old1 = coeff[1];
        seq_t old2 = coeff[2];
        seq_t old3 = coeff[3];

        /* Multiply by (x + d)  and keep only first four coefficients. */
        coeff[0] = d * old0;
        coeff[1] = old0 + d * old1;
        coeff[2] = old1 + d * old2;
        coeff[3] = old2 + d * old3;

        /* Cache the coefficient of x^3 for this n. */
        cache[m] = coeff[3];
    }

    last_n = n;
    return cache[n];
}

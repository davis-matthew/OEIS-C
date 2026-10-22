/*
 * A381846  Area of the unique primitive Pythagorean triple (a,b,c) such that
 *          (a-b+c)/2 = A000108(n) and its long leg and hypotenuse are
 *          consecutive natural numbers.
 * https://oeis.org/A381846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C_n * (C_n - 1) * (2*C_n - 1) where C_n is the nth
 *            Catalan number (C_0 = 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
/* The build supplies typedefs for seq_t and idx_t */
#include "../utils/binom.h"     /* binomial(idx_t n, idx_t k) returns seq_t */

/* Static cache for Catalan numbers */
static seq_t *cat_cache = NULL;    /* array holding Catalan numbers */
static idx_t cat_len   = 0;        /* number of valid entries in cat_cache */
static idx_t cat_cap   = 0;        /* current allocated capacity of cat_cache */

/* Ensure the cache can hold at least 'min_cap' entries */
static void ensure_cat_capacity(idx_t min_cap)
{
    if (min_cap <= cat_cap) return;

    idx_t new_cap = cat_cap ? cat_cap << 1 : 8;
    while (new_cap < min_cap) new_cap <<= 1;   /* double until enough */

    seq_t *tmp = (seq_t *)realloc(cat_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                /* allocation failed */
    cat_cache = tmp;
    cat_cap = new_cap;
}

/* Compute the nth Catalan number, caching intermediate results.
   Uses the recurrence C_0 = 1,
   C_n = C_{n-1} * (4*n - 2) / (n + 1). */
static seq_t get_catalan(idx_t n)
{
    if (n < cat_len) return cat_cache[n];

    if (cat_len == 0) {
        ensure_cat_capacity(1);
        cat_cache[0] = (seq_t)1;
        cat_len = 1;
    }

    for (idx_t i = cat_len; i <= n; ++i) {
        seq_t prev   = cat_cache[i - 1];
        seq_t numer  = prev * ((seq_t)(4 * i - 2));  /* 4*i - 2 = 2*(2i-1) */
        seq_t denom  = (seq_t)(i + 1);
        seq_t cur    = numer / denom;                /* exact division */
        ensure_cat_capacity(i + 1);
        cat_cache[i] = cur;
    }
    cat_len = n + 1;
    return cat_cache[n];
}

/* Main sequence function */
seq_t a(idx_t n)
{
    seq_t cat = get_catalan(n);            /* C_n */
    seq_t cat_minus1 = cat - (seq_t)1;
    seq_t two_cat_minus1 = cat * (seq_t)2 - (seq_t)1;

    return cat * cat_minus1 * two_cat_minus1;
}

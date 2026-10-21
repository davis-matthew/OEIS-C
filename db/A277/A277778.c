/*
 * A277778  If n is even, a(n) = a(n/2-1), and if n is odd, a(n) = a((n-1)/2)
 *          - a((n+1)/2), with a(1) = a(2) = 1.
 * https://oeis.org/A277778
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = a(2) = 1;
   for n > 2, if n is even: a(n) = a(n/2-1);
   if n is odd : a(n) = a((n-1)/2) - a((n+1)/2); */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static seq_t  *cache_vals  = NULL;
static unsigned char *cache_flag = NULL;
static size_t   cache_cap  = 0;   /* number of entries allocated */

/* Ensure cache can hold indices up to at least n (0‑based). */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n < cache_cap)
        return;

    size_t new_cap = cache_cap ? cache_cap : 64;
    /* make new_cap > n */
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    seq_t *tmp_vals  = (seq_t *)realloc(cache_vals,
                                        new_cap * sizeof(seq_t));
    unsigned char *tmp_flag =
        (unsigned char *)realloc(cache_flag,
                                 new_cap * sizeof(unsigned char));
    if (!tmp_vals || !tmp_flag)
        abort();              /* out of memory */

    /* initialise new flag entries as “not computed” */
    for (size_t i = cache_cap; i < new_cap; ++i)
        tmp_flag[i] = 0;

    cache_vals  = tmp_vals;
    cache_flag  = tmp_flag;
    cache_cap   = new_cap;
}

/* Recursive, memoised evaluation of the recurrence. */
static seq_t a_rec(idx_t n)
{
    if ((size_t)n < cache_cap && cache_flag[n])
        return cache_vals[n];

    seq_t val;
    if (n == 1 || n == 2)
        val = 1;
    else if ((n & 1U) == 0)            /* even n */
        val = a_rec(n / 2 - 1);
    else                               /* odd n */
        val = a_rec((n - 1) / 2) - a_rec((n + 1) / 2);

    ensure_capacity(n);
    cache_vals[n] = val;
    cache_flag[n] = 1;
    return val;
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (n < 1)   /* undefined for n == 0, but guard against it */
        return 0;
    return a_rec(n);
}

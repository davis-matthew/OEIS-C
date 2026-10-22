/*
 * A364898  Decimal expansion of the number of kilograms per cubic meter
 *          (kg/m^3) in 1 pound per cubic foot (lb/ft^3).
 * https://oeis.org/A364898
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = decimal digit of (0.45359237 / 0.3048^3) starting from
 *            the first digit of the integer part (a(2)=1)
 * Range:     u8 n <= 2, u16 none, u32 n <= 2, u64 n <= 100, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* The reduced rational value of 0.45359237/(0.3048^3) is
   28349523125 / 1769802912. */
static const seq_t NUM = (seq_t)28349523125ULL;   /* numerator   */
static const seq_t DEN = (seq_t)1769802912ULL;    /* denominator */

/* Cache of computed digits: cache[i] holds a(i).
   Indices 0 and 1 are unused so that a(2) is the first stored digit. */
static seq_t *cache      = NULL;   /* allocated array of digits */
static idx_t  cache_size = 0;      /* current allocated length   */
static idx_t  last       = 1;      /* largest index already
                                      computed and stored       */
static seq_t  rem        = 0;      /* remainder for the fractional part */
static bool   int_done   = false;  /* have we processed the integer part? */

/* Ensure that cache has room for indices up to 'need'. */
static void ensure_capacity(idx_t need)
{
    if (need < cache_size)
        return;
    idx_t new_size = cache_size ? cache_size * 2 : 8;
    if (new_size <= need)
        new_size = need + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache)
        abort();   /* out of memory */
    cache = new_cache;
    cache_size = new_size;
}

/* Compute digits up to index 'n' (inclusive). */
static void compute_up_to(idx_t n)
{
    if (n <= last)
        return;        /* already computed */

    ensure_capacity(n);   /* make space for new digits */

    /* Process integer part once. */
    if (!int_done) {
        seq_t int_part = NUM / DEN; /* integer part of the value */

        /* Extract decimal digits of int_part. */
        seq_t tmp[20];
        idx_t cnt = 0;
        seq_t v = int_part;
        if (v == 0) {
            tmp[cnt++] = 0;
        } else {
            while (v > 0) {
                tmp[cnt++] = v % 10;
                v /= 10;
            }
        }

        /* Store digits starting at cache[2] (a(2)). */
        for (idx_t i = 0; i < cnt; ++i)
            cache[2 + i] = tmp[cnt - 1 - i];

        last = 2 + cnt - 1;  /* last index after integer digits */
        rem = NUM % DEN;     /* remainder for fractional part     */
        int_done = true;
    }

    /* Compute fractional digits up to index 'n'. */
    for (idx_t idx = last + 1; idx <= n; ++idx) {
        rem = rem * 10;
        seq_t d = rem / DEN;
        rem = rem % DEN;
        cache[idx] = d;
        last = idx;
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 2)          /* behaviour below offset is undefined */
        return (seq_t)0;

    compute_up_to(n);
    return cache[n];
}

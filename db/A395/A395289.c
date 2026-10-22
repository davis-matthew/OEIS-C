/*
 * A395289  a(n) = (2*n)! * [x^(2*n)] cos(2*x) * cos(3*x) * cos(4*x) /
 *          cos(24*x).
 * https://oeis.org/A395289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^n * (1 + 9^n + 25^n + 81^n)/4 - Sum_{k=0..n-1}
 *            (-576)^(n-k) * binomial(2*n, 2*k) * a(k)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the recurrence given in the OEIS
   description.  Results are cached, so repeated calls are efficient.
   Powers of 9, 25, 81 and 576 are also pre‑computed incrementally. */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Static cache for a(k) values */
static seq_t *cache = NULL;          /* pointer to a(k) array            */
static size_t cache_len = 0;         /* number of elements stored        */

/* Static tables for powers of 9, 25, 81, 576 */
static seq_t *pow9  = NULL;
static seq_t *pow25 = NULL;
static seq_t *pow81 = NULL;
static seq_t *pow576 = NULL;
static size_t pow_len = 0;           /* store indices 0..pow_len-1        */

/* Ensure cache has room for entries up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= cache_len)
        return;
    size_t new_len = (size_t)n + 1;
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    for (size_t i = cache_len; i < new_len; ++i)
        cache[i] = 0;
    cache_len = new_len;
}

/* Ensure power tables have room up to index n (inclusive)
   and fill any new entries. */
static void ensure_powers(idx_t n)
{
    if ((size_t)n + 1 <= pow_len)
        return;
    size_t new_len = (size_t)n + 1;
    pow9  = (seq_t *)realloc(pow9,  new_len * sizeof(seq_t));
    pow25 = (seq_t *)realloc(pow25, new_len * sizeof(seq_t));
    pow81 = (seq_t *)realloc(pow81, new_len * sizeof(seq_t));
    pow576= (seq_t *)realloc(pow576,new_len * sizeof(seq_t));
    for (size_t i = pow_len; i < new_len; ++i) {
        if (i == 0) {
            pow9[i]   = 1;
            pow25[i]  = 1;
            pow81[i]  = 1;
            pow576[i] = 1;
        } else {
            pow9[i]   = pow9[i-1]   * (seq_t)9;
            pow25[i]  = pow25[i-1]  * (seq_t)25;
            pow81[i]  = pow81[i-1]  * (seq_t)81;
            pow576[i] = pow576[i-1] * (seq_t)576;
        }
    }
    pow_len = new_len;
}

/* Main function */
seq_t a(idx_t n)
{
    /* Initialise cache and first term.  a(0) = 1. */
    if (cache_len == 0) {
        ensure_cache(0);
        cache[0] = 1;
        ensure_powers(0);
    }

    /* Ensure storage for requested index */
    ensure_cache(n);
    if (cache_len > (size_t)n + 1)  /* value already computed */
        return cache[n];

    /* Compute terms up to n */
    for (idx_t i = 1; i <= n; ++i) {
        ensure_powers(i);

        /* term1 = (-1)^i * (1 + 9^i + 25^i + 81^i) / 4 */
        seq_t ps = 1 + pow9[i] + pow25[i] + pow81[i];
        seq_t term1 = ps / 4;
        if (i & 1)   /* odd i → negative */
            term1 = -term1;

        /* Sum part: sum_{k=0}^{i-1} (-576)^(i-k) * bin(2i,2k) * a(k) */
        seq_t part_pos = 0;  /* accumulate positive terms   */
        seq_t part_neg = 0;  /* accumulate negative terms   */

        seq_t bin = 1;                               /* bin(2i,0) */
        seq_t abs_pow = pow576[i];                    /* |(-576)^i| */
        bool neg_flag = (i & 1);                      /* sign of first term */

        for (idx_t k = 0; k < i; ++k) {
            /* termAbs = bin * a(k) * abs_pow  (always non‑negative) */
            seq_t termAbs = bin * cache[k];
            termAbs *= abs_pow;

            if (neg_flag)
                part_neg += termAbs;
            else
                part_pos += termAbs;

            if (k + 1 == i)
                break; /* ready for next i */

            /* Update bin to bin(2i,2(k+1)) */
            seq_t num = bin * (seq_t)(2*i - 2*k) * (seq_t)(2*i - 2*k - 1);
            seq_t den = (seq_t)(2*k + 2) * (seq_t)(2*k + 1);
            bin = num / den;  /* exact division */

            /* Prepare next abs_pow = abs_pow / 576 */
            abs_pow /= (seq_t)576;

            /* Flip sign for next exponent (i-(k+1)) */
            neg_flag = !neg_flag;
        }

        /* total sum = part_pos - part_neg  (may be negative) */
        seq_t sum = part_pos - part_neg;

        /* a(i) = term1 - sum */
        seq_t ai = term1 - sum;

        cache[i] = ai;
    }

    return cache[n];
}

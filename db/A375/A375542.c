/*
 * A375542  a(n) = n! * [x^n] (-2 - exp(-x))^n.
 * https://oeis.org/A375542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * 2^(n-k) * k^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_seq.h"

static seq_t *pow2_cache = NULL;      /* 2^i  for i = 0..max_precalc */
static size_t pow2_max = 0;           /* highest index computed */

static seq_t *a_cache   = NULL;      /* computed a(n) values */
static size_t a_max     = 0;          /* largest n stored in a_cache */

/* ensure that pow2_cache contains 2^i for 0 <= i <= n */
static void ensure_pow2(idx_t n)
{
    size_t sn = (size_t)n;
    if (pow2_cache == NULL) {
        size_t sz = sn + 1;
        pow2_cache = (seq_t*)malloc(sz * sizeof(seq_t));
        if (!pow2_cache) abort();
        pow2_cache[0] = (seq_t)1;
        for (size_t i = 1; i <= sn; ++i)
            pow2_cache[i] = pow2_cache[i-1] * (seq_t)2;
        pow2_max = sn;
    } else if (sn > pow2_max) {
        size_t old = pow2_max;
        size_t sz = sn + 1;
        seq_t *tmp = (seq_t*)realloc(pow2_cache, sz * sizeof(seq_t));
        if (!tmp) abort();
        pow2_cache = tmp;
        for (size_t i = old + 1; i <= sn; ++i)
            pow2_cache[i] = pow2_cache[i-1] * (seq_t)2;
        pow2_max = sn;
    }
}

/* compute a(n) with memoisation and O(n) per new value */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    size_t sn = (size_t)n;
    /* initialise cache on first use */
    if (a_cache == NULL) {
        a_cache = (seq_t*)malloc(sizeof(seq_t));   /* size 1 for a(0) */
        if (!a_cache) abort();
        a_cache[0] = (seq_t)1;
        a_max = 0;
    }

    /* return from cache if already computed */
    if (sn <= a_max)
        return a_cache[sn];

    /* extend cache to hold all values up to sn */
    size_t new_sz = sn + 1;
    seq_t *tmp = (seq_t*)realloc(a_cache, new_sz * sizeof(seq_t));
    if (!tmp) abort();
    a_cache = tmp;

    /* compute new values from a_max+1 .. sn */
    for (size_t i = a_max + 1; i <= sn; ++i) {
        if (i == 0) {
            a_cache[0] = (seq_t)1;
            continue;
        }

        ensure_pow2(i);

        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;      /* binomial(i,0) */

        for (size_t k = 0; k <= i; ++k) {
            /* term = binom * 2^(i-k) * k^i */
            seq_t term = binom;
            term *= pow2_cache[i - k];
            seq_t kpwr = pow_seq((seq_t)k, (int)i);
            term *= kpwr;
            sum += term;

            /* update binom for next k, avoid division by zero */
            if (k < i) {
                /* binom(i,k+1) = binom(i,k) * (i-k) / (k+1) */
                binom = (binom * (seq_t)(i - k)) / (seq_t)(k + 1);
            }
        }

        a_cache[i] = sum;
    }

    a_max = sn;
    return a_cache[sn];
}

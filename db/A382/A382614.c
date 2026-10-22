/*
 * A382614  Expansion of 1/(1 - x*(1 + x)^3)^3.
 * https://oeis.org/A382614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(k+2,2) * binomial(3*k, n-k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 19, u64 n <= 40, u128 n <= 85,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (valid for n >= 12):
   a(n) = 3*a(n-1) + 6*a(n-2) - 8*a(n-3) - 33*a(n-4) - 24*a(n-5)
          + 39*a(n-6) + 108*a(n-7) + 123*a(n-8) + 84*a(n-9)
          + 36*a(n-10) + 9*a(n-11) + a(n-12)       */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/binom.h"

static seq_t *seq_cache = NULL;   /* cached values of a(n) */
static size_t seq_size  = 0;      /* first unused index (size of used part) */
static size_t seq_cap   = 0;      /* allocated capacity */

/* helper: guarantee that the cache holds values up to at least idx n */
static void
ensure_cache_upto(idx_t n)
{
    if (seq_cache == NULL) {
        /* allocate a reasonable initial capacity */
        seq_cap = n + 1;
        if (seq_cap < 16)
            seq_cap = 16;
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_size = 0;
    }
    else if (seq_cap < (size_t)(n + 1)) {
        size_t new_cap = seq_cap * 2;
        if (new_cap < (size_t)(n + 1))
            new_cap = n + 1;
        seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }

    /* compute needed entries */
    for (idx_t i = seq_size; i <= n; ++i) {
        if (i <= 12) {
            /* use binomial sum definition for the first 13 terms */
            seq_t val = 0;
            for (idx_t k = 0; k <= i; ++k) {
                seq_t t1 = binom(k + 2, 2);      /* binomial(k+2,2) */
                seq_t t2 = binom(3 * k, i - k);   /* binomial(3*k,i-k) */
                val += t1 * t2;
            }
            seq_cache[i] = val;
        } else {
            /* use the linear recurrence for later terms */
            seq_t val = 0;
            val += (seq_t)3  * seq_cache[i - 1];
            val += (seq_t)6  * seq_cache[i - 2];
            val -= (seq_t)8  * seq_cache[i - 3];
            val -= (seq_t)33 * seq_cache[i - 4];
            val -= (seq_t)24 * seq_cache[i - 5];
            val += (seq_t)39 * seq_cache[i - 6];
            val += (seq_t)108 * seq_cache[i - 7];
            val += (seq_t)123 * seq_cache[i - 8];
            val += (seq_t)84  * seq_cache[i - 9];
            val += (seq_t)36  * seq_cache[i - 10];
            val += (seq_t)9   * seq_cache[i - 11];
            val += seq_cache[i - 12];
            seq_cache[i] = val;
        }
    }
    seq_size = n + 1;
}

seq_t
a(idx_t n)
{
    ensure_cache_upto(n);
    return seq_cache[n];
}

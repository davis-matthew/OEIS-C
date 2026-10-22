/*
 * A367983  Expansion of e.g.f. exp(-3*x) / (2 - exp(4*x)).
 * https://oeis.org/A367983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>=0} (4*k-3)^n / 2^(k+1) Recurrence used: a(0) = 1
 *            a(n) = (-3)^n + Sum_{k=1..n} binom(n,k) * 4^k * a(n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper functions for dynamic allocation of arbitrary‑precision
   integer arrays (provided by the build environment). */
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static seq_t *cache = NULL;          /* stored a(0)..a(k) values */
static size_t cap   = 0;              /* current allocated size of 'cache' */
static seq_t pow3_current = 1;        /* 3^i for the last i computed */
static idx_t  pow3_n = 0;             /* the exponent associated with pow3_current */

/* Expand the cache to at least index n.  All newly
   computed values use only integer arithmetic. */
static void ensure_size(idx_t n)
{
    if ((idx_t)cap > n) return;      /* already large enough */

    size_t new_cap = (size_t)n + 1;
    seq_t *new_cache = alloc_seq_array(new_cap);
    /* copy old values */
    for (size_t i = 0; i < cap; ++i) new_cache[i] = cache ? cache[i] : 0;

    if (cap == 0) {                  /* initialise first term a(0) = 1 */
        new_cache[0] = (seq_t)1;
        pow3_current = (seq_t)1;     /* 3^0 */
        pow3_n = 0;
        cap = 1;
    }

    for (idx_t i = (idx_t)cap; i <= n; ++i) {
        /* compute 3^i incrementally */
        pow3_current *= (seq_t)3;     /* pow3_current = 3^i */

        /* sum_{k=1..i} binom(i,k)*4^k*a(i-k) */
        seq_t sum = 0;
        seq_t binom = 1;             /* binom(i,0) = 1 */
        seq_t pow4_k = 1;            /* 4^0 */

        for (idx_t k = 1; k <= i; ++k) {
            /* update binom(i,k) from binom(i,k-1) */
            binom = binom * (seq_t)(i - k + 1);
            binom = binom / (seq_t)k;

            /* next power of 4 */
            pow4_k *= (seq_t)4;      /* now pow4_k = 4^k */

            /* term = binom * 4^k * a(i-k) */
            seq_t term = binom * pow4_k;
            term = term * new_cache[i - k];
            sum += term;
        }

        /* a(i) = sum + (-3)^i */
        if ((i & 1) == 0) {
            new_cache[i] = sum + pow3_current;   /* (-3)^i = +3^i */
        } else {
            new_cache[i] = sum - pow3_current;   /* (-3)^i = -3^i */
        }
    }

    /* free old cache and adopt new one */
    if (cache) free_seq_array(cache);
    cache = new_cache;
    cap   = new_cap;
    pow3_n = n;
}

seq_t a(idx_t n)
{
    ensure_size(n);
    return cache[n];
}

/*
 * A355380  Expansion of e.g.f. exp(exp(3*x) + exp(2*x) - 2).
 * https://oeis.org/A355380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (3^k + 2^k) * binomial(n-1, k-1) * a(n-k)
 *            Initial value: a(0) = 1 The implementation uses dynamic arrays
 *            and helper routines for arbitrary‑precision integers. It is safe
 *            for all seq_t implementations (primitive types or big integers)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Helpers:   mul_small, seq_mul, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_realloc.h"
#include "../utils/mul_small.h"
#include "../utils/seq_mul.h"

/* Helper: convert a primitive unsigned long long to seq_t */
static inline seq_t to_seq(unsigned long long x) { return (seq_t)x; }

/* Global static state for memoisation */
static seq_t *a_cache  = NULL; /* a(n) values          */
static size_t a_cap    = 0;
static idx_t  a_len    = 0;

static seq_t *pow2_cache = NULL; /* 2^k */
static size_t pow2_cap   = 0;
static idx_t  pow2_len   = 0;

static seq_t *pow3_cache = NULL; /* 3^k */
static size_t pow3_cap   = 0;
static idx_t  pow3_len   = 0;

/* Ensure all auxiliary arrays are large enough and all a(i) for i <= n are computed.  */
static void ensure_up_to(idx_t n)
{
    if (n < a_len) return;                /* already computed */

    /* Grow power‑2 cache */
    if (pow2_cap < (size_t)(n + 1)) {
        seq_t *tmp = seq_realloc(pow2_cache, pow2_cap, (size_t)(n + 1));
        pow2_cache = tmp;
        pow2_cap   = (size_t)(n + 1);
    }
    if (pow2_len == 0) {                  /* first allocation */
        pow2_cache[0] = to_seq(1);
        pow2_len      = 1;
    }
    for (idx_t i = pow2_len; i <= n; ++i)
        pow2_cache[i] = mul_small(pow2_cache[i - 1], 2);
    pow2_len = n + 1;

    /* Grow power‑3 cache */
    if (pow3_cap < (size_t)(n + 1)) {
        seq_t *tmp = seq_realloc(pow3_cache, pow3_cap, (size_t)(n + 1));
        pow3_cache = tmp;
        pow3_cap   = (size_t)(n + 1);
    }
    if (pow3_len == 0) {
        pow3_cache[0] = to_seq(1);
        pow3_len      = 1;
    }
    for (idx_t i = pow3_len; i <= n; ++i)
        pow3_cache[i] = mul_small(pow3_cache[i - 1], 3);
    pow3_len = n + 1;

    /* Grow a_cache */
    if (a_cap < (size_t)(n + 1)) {
        seq_t *tmp = seq_realloc(a_cache, a_cap, (size_t)(n + 1));
        a_cache = tmp;
        a_cap   = (size_t)(n + 1);
    }
    if (a_len == 0) {                     /* a(0) = 1 */
        a_cache[0] = to_seq(1);
        a_len      = 1;
    }

    /* Compute a(i) for all new i */
    for (idx_t i = a_len; i <= n; ++i) {
        seq_t sum   = 0;
        seq_t binom = 1;                /* binomial(i-1,0) */

        for (idx_t k = 1; k <= i; ++k) {
            /* term = a(i-k) * binom * (2^k + 3^k) */
            seq_t term = a_cache[i - k];
            term = seq_mul(term, binom);

            seq_t power = pow2_cache[k];
            power += pow3_cache[k];
            term = seq_mul(term, power);

            sum += term;

            /* Update binom for next k: binom = binom * (i - k) / k   */
            if (k < i) {
                seq_t num = seq_mul(binom, to_seq(i - k));
                binom = num / to_seq(k);
            }
        }
        a_cache[i] = sum;
    }
    a_len = n + 1;
}

seq_t a(idx_t n)
{
    ensure_up_to(n);
    return a_cache[n];
}

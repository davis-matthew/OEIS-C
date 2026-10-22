/*
 * A355767  E.g.f. satisfies A(x)^(A(x)^2) = 1/(1 - x*A(x)).
 * https://oeis.org/A355767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 17, i128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A355767
   Formula: a(n) = Sum_{k=0..n} (n-2*k+1)^(k-1) * |Stirling1(n,k)|
   (Stirling1 are signed Stirling numbers of the first kind) */

/* Standard library includes */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* helper: integer exponentiation (base may be negative) */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

/* dynamic cache for a(n) */
static seq_t *a_cache   = NULL;
static idx_t   a_cap    = 0;
static idx_t   a_size   = 0;

/* dynamic cache for Stirling numbers of the first kind (signed) */
static seq_t *stirling  = NULL;
static idx_t   s_cap    = 0;

/* ensure capacity for a_cache */
static void ensure_a_cache(idx_t need)
{
    if (need < a_cap)
        return;
    idx_t new_cap = a_cap ? a_cap << 1 : 4;
    while (new_cap <= need)
        new_cap <<= 1;
    a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
    a_cap   = new_cap;
}

/* ensure capacity for stirling */
static void ensure_stirling(idx_t need)
{
    if (need < s_cap)
        return;
    idx_t new_cap = s_cap ? s_cap << 1 : 4;
    while (new_cap <= need)
        new_cap <<= 1;
    stirling = (seq_t *)realloc(stirling, new_cap * sizeof(seq_t));
    s_cap    = new_cap;
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0) {
        return 1;
    }

    /* initialise caches on first call */
    if (a_size == 0) {
        ensure_a_cache(1);
        a_cache[0] = 1;             /* a(0) = 1 */
        a_size = 1;

        ensure_stirling(1);
        stirling[0] = 1;             /* s(0,0) = 1 */
    }

    /* return cached value if available */
    if (n < a_size)
        return a_cache[n];

    /* extend caches up to n */
    for (idx_t i = a_size; i <= n; ++i) {
        /* prepare Stirling row for i */
        ensure_stirling(i + 1);      /* indices 0..i */
        stirling[i] = 0;             /* s(i,i) = 0 for i>0 initially */
        for (idx_t k = i; k >= 1; --k) {
            seq_t prev_k     = stirling[k];       /* s(i-1,k)  */
            seq_t prev_k_1   = stirling[k-1];     /* s(i-1,k-1) */
            stirling[k]     = prev_k_1 + (seq_t)(i - 1) * prev_k;
        }
        stirling[0] = 0;             /* s(i,0) = 0 for i>0 */

        /* compute a(i) */
        seq_t ai = 0;
        for (idx_t k = 1; k <= i; ++k) {
            /* base = i-2*k+1, may be negative */
            seq_t base = (seq_t)((int64_t)i - 2 * (int64_t)k + 1);
            seq_t pow_val = (k == 1) ? 1 : pow_seq(base, k - 1);

            /* use absolute value of Stirling number */
            seq_t s = stirling[k];
            if (s < 0)
                s = -s;

            ai += pow_val * s;
        }

        /* store result */
        ensure_a_cache(i);
        a_cache[i] = ai;
        a_size     = i + 1;
    }

    return a_cache[n];
}

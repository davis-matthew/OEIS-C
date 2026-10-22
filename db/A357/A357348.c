/*
 * A357348  E.g.f. satisfies A(x) = (exp(x * exp(A(x))) - 1) * exp(3 * A(x)).
 * https://oeis.org/A357348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (n+3*k)^(k-1) * Stirling2(n, k)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   big_pow, mul_small, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"    /* seq_t big_pow(seq_t base, seq_t exp) */
#include "../utils/mul_small.h"  /* seq_t mul_small(seq_t x, unsigned int m) */
#include "../utils/seq_mul.h"    /* seq_t seq_mul(seq_t a, seq_t b) */

/* Memoised cache of Stirling numbers for the previous row  */
static seq_t *S_prev = NULL;
static size_t  S_prev_len = 0;

/* Memoised result array  */
static seq_t *a_cache = NULL;
static size_t a_cache_len = 0;

/* Return the n-th term of sequence A357348.  */
seq_t a(idx_t n)
{
    /* Initialise the caches on the first call.  */
    if (a_cache_len == 0) {
        a_cache_len = 1;
        a_cache = (seq_t *)malloc(a_cache_len * sizeof(seq_t));
        a_cache[0] = (seq_t)0;                /* a(0) = 0 */

        S_prev_len = 1;
        S_prev = (seq_t *)malloc(S_prev_len * sizeof(seq_t));
        S_prev[0] = (seq_t)1;                /* S(0,0) = 1 */
    }

    /* Return cached value if already computed.  */
    if ((idx_t)n < (idx_t)a_cache_len) {
        return a_cache[n];
    }

    /* Compute terms from the current cache up to n.  */
    for (idx_t cur = (idx_t)a_cache_len; cur <= n; ++cur) {
        size_t new_len = (size_t)cur + 1;
        seq_t *S_cur = (seq_t *)malloc(new_len * sizeof(seq_t));
        S_cur[0] = (seq_t)0;  /* S(cur,0) = 0 for cur > 0 */

        /* Build current Stirling row from previous row.  */
        for (idx_t k = 1; k <= cur; ++k) {
            seq_t term1 = 0;
            if ((size_t)k < S_prev_len) {
                term1 = mul_small(S_prev[(size_t)k], (unsigned int)k);
            }
            seq_t term2 = 0;
            if ((size_t)(k - 1) < S_prev_len) {
                term2 = S_prev[(size_t)(k - 1)];
            }
            S_cur[(size_t)k] = term1 + term2;
        }

        /* Accumulate the sum for a(cur).  */
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= cur; ++k) {
            seq_t base  = (seq_t)cur + (seq_t)3 * (seq_t)k;
            seq_t exp   = (seq_t)(k - 1);
            seq_t power = big_pow(base, exp);
            seq_t term  = seq_mul(S_cur[(size_t)k], power);
            sum += term;
        }

        /* Extend the result cache if necessary.  */
        if ((size_t)(cur + 1) > a_cache_len) {
            seq_t *new_cache = (seq_t *)realloc(
                                   a_cache, (size_t)(cur + 1) * sizeof(seq_t));
            a_cache = new_cache;
            a_cache_len = (size_t)(cur + 1);
        }
        a_cache[(size_t)cur] = sum;

        /* Prepare for next iteration.  */
        free(S_prev);
        S_prev     = S_cur;
        S_prev_len = new_len;
    }

    return a_cache[(size_t)n];
}

/*
 * A355131  E.g.f. A(x) satisfies A(x) = 1 + 2 * (exp(x) - 1) * A(2 * (exp(x)
 *          - 1)).
 * https://oeis.org/A355131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n ≥ 1: a(n) = Σ_{k=1..n} k * 2^k * S(n,k) * a(k-1)
 *            where S(n,k) are Stirling numbers of the second kind. The
 *            function computes terms on demand, memoising all previous values
 *            up to the requested index. The implementation runs in O(n²) time
 *            and O(n) memory, suitable for n ≤ 10 000
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 1,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 13 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* the code below runs in wide_t: 128 bits at the fixed widths (never narrower
   than seq_t), seq_t itself for big, so values that pass 2^64 on the way to a
   term that fits stay exact; a() returns the term in seq_t */
#ifdef __cplusplus
typedef seq_t wide_t;
#else
typedef __typeof__((seq_t)0 + (__int128)0) wide_t;
#endif
#define seq_t wide_t

seq_t a_wide(idx_t n)
{
    /* static cache for the sequence values */
    static seq_t *a_cache   = NULL;   /* a_cache[i] = a_wide(i) */
    static size_t a_cap     = 0;       /* capacity of a_cache    */
    static idx_t  a_len     = 0;       /* number of values stored */

    /* static array for the current Stirling row S(k) = S(cur,k) */
    static seq_t *S          = NULL;   /* S[k] is S(cur,k)        */
    static size_t S_cap      = 0;      /* capacity of S           */

    /* initialise on first call */
    if (a_len == 0) {
        /* a_wide(0) = 1 */
        a_cap = 1;
        a_cache = mem_new(seq_t, a_cap);
        a_cache[0] = 1;
        a_len = 1;

        /* S(0,0) = 1 */
        S_cap = 1;
        S = mem_new(seq_t, S_cap);
        S[0] = 1;
    }

    /* grow the caches to accommodate values up to n */
    while (a_len <= n) {
        idx_t cur = a_len;              /* compute next value a_wide(cur) */

        /*-- Update Stirling numbers to row cur --------------------------------*/
        if (cur + 1 > S_cap) {
            size_t new_cap = S_cap ? S_cap * 2 : 4;
            while (new_cap < cur + 1) new_cap <<= 1;
            S = mem_resize(seq_t, S, new_cap);
            S_cap = new_cap;
        }

        /* in-place update: S[k] = k*S[k] + S[k-1]  for k=cur..1 */
        for (idx_t k = cur; k >= 1; --k) {
            S[k] = k * S[k] + S[k - 1];
        }
        S[0] = 0;   /* S(cur,0) = 0 for cur > 0 */

        /*-- Compute a_wide(cur) ---------------------------------------------*/
        seq_t sum = 0;
        seq_t pow2 = 1;   /* 2^0 */
        for (idx_t k = 1; k <= cur; ++k) {
            pow2 = pow2 * 2;          /* pow2 = 2^k */
            seq_t term = a_cache[k - 1];
            term = term * pow2;
            term = term * S[k];
            term = term * k;
            sum = sum + term;
        }

        /* store the result */
        if (cur + 1 > a_cap) {
            size_t new_cap = a_cap ? a_cap * 2 : 4;
            while (new_cap < cur + 1) new_cap <<= 1;
            a_cache = mem_resize(seq_t, a_cache, new_cap);
            a_cap = new_cap;
        }
        a_cache[cur] = sum;
        ++a_len;
    }

    return a_cache[n];
}

#undef seq_t
seq_t a(idx_t n)
{
    return (seq_t)a_wide(n);
}

/*
 * A308330  a(n) = n! * [x^n] exp(exp(n*x)/(1 - x) - 1).
 * https://oeis.org/A308330
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * [x^n] exp(exp(n*x)/(1 - x) - 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* allocate space for c_k = k! * Σ_{r=0}^{k} n^r/r!   (k = 0..n) */
    seq_t *c = mem_new(seq_t, (size_t)(n + 1));
    seq_t *B = mem_new(seq_t, (size_t)(n + 1));
    if (!c || !B) { /* out of memory – return 0 */
        mem_free(c);
        mem_free(B);
        return (seq_t)0;
    }

    /* c[0] = 0! * (n^0/0!) = 1 */
    c[0] = (seq_t)1;

    /* powers of n : pow_n = n^k */
    seq_t pow_n = (seq_t)1;          /* n^0 */
    seq_t n_seq = (seq_t)n;
    for (idx_t k = 1; k <= n; ++k) {
        pow_n = pow_n * n_seq;       /* n^k */
        /* c_k = k * c_{k-1} + n^k */
        seq_t k_seq = (seq_t)k;
        c[k] = k_seq * c[k - 1] + pow_n;
    }

    /* Bell polynomial recurrence: B_0 = 1 */
    B[0] = (seq_t)1;
    for (idx_t m = 1; m <= n; ++m) {
        seq_t sum  = (seq_t)0;
        seq_t binom = (seq_t)1;       /* binom(m-1,0) */
        for (idx_t k = 1; k <= m; ++k) {
            /* term = binom * c_k * B_{m-k} */
            seq_t prod = binom * c[k];
            prod = prod * B[m - k];
            sum += prod;

            if (k < m) {
                /* update binom to binom(m-1, k) */
                seq_t mult = (seq_t)(m - k);
                seq_t divv = (seq_t)k;
                binom = (binom * mult) / divv;   /* exact division */
            }
        }
        B[m] = sum;
    }

    seq_t result = B[n];
    mem_free(c);
    mem_free(B);
    return result;
}

/*
 * A318130  Number of sets of subsets of {1,...,n} with intersection {}.
 * https://oeis.org/A318130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^{2^n} + Σ_{k=1}^{n} (-1)^k * binom(n,k) * 2^{2^{n-k}} +
 *            1, a(0)=2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)2;
    /* The following implementation works safely for n < 64 */
    /* For higher n the numbers grow astronomically and cannot be stored in memory. */
    /* If required, a fallback or a different method would be needed. */

    /* Compute 2^{2^n} using pow2(2^n). */
    idx_t exp_full = 1ULL << n;    /* 2^n  (n < 64 guaranteed here) */
    seq_t res = pow2(exp_full);    /* 2^{2^n} */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t bin = binom(n, k);               /* binomial coefficient */
        idx_t exp_part = 1ULL << (n - k);       /* 2^{n-k} */
        seq_t pow_term = pow2(exp_part);        /* 2^{2^{n-k}} */
        seq_t term = bin * pow_term;

        if (k & 1)          /* odd k → subtract */
            res -= term;
        else                /* even k → add */
            res += term;
    }

    res += (seq_t)1;                 /* add the final +1 term */
    return res;
}

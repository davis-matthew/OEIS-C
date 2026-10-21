/*
 * A287065  Number of dominating sets on the n X n rook graph.
 * https://oeis.org/A287065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^n-1)^n + Sum_{c=1..n} (-1)^(c+1) * binomial(n,c) *
 *            (2^(n-c)-1)^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   big_pow, binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include needed utilities */
#include <stdbool.h>
#include "../utils/pow2.h"      /* pow2(idx_t exp)   */
#include "../utils/big_pow.h"   /* big_pow(seq_t base, seq_t exp) */
#include "../utils/binomial.h"  /* binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* Undefined below offset, return 0 */

    /* 2^n as seq_t */
    seq_t pow2n = pow2(n);

    /* Base term (2^n - 1)^n */
    seq_t base_main = pow2n - (seq_t)1;
    seq_t ans = big_pow(base_main, n);

    /* Add contributions from c = 1 .. n */
    for (idx_t c = 1; c <= n; ++c) {
        seq_t pow2_val = pow2(n - c);              /* 2^(n-c) */
        seq_t base = pow2_val - (seq_t)1;          /* (2^(n-c) - 1) */
        seq_t pow_term = big_pow(base, n);         /* (2^(n-c) - 1)^n */
        seq_t term = binomial(n, c) * pow_term;    /* binomial(n,c) * pow_term */

        if (c & 1) /* c odd: add */
            ans += term;
        else       /* c even: subtract */
            ans -= term;
    }
    return ans;
}

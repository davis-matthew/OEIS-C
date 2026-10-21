/*
 * A260619  Arithmetic derivative of hyperfactorial(n).
 * https://oeis.org/A260619
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = arithmetic_derivative( hyperfactorial(n) ), where
 *            hyperfactorial H(n) = ∏_{k=1}^{n} k^k
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 5, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses the recurrence
   H(n)   = H(n-1) * n^n
   D(n)   = arithmetic derivative of n
   a(n) = D( H(n) ) = n^n * ( H(n-1) * D(n) + D( H(n-1) ) )
   computed iteratively for i = 1 .. n. */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

/* Compute arithmetic derivative of a positive integer m (m >= 1).  */
static seq_t deriv_of_integer(idx_t m)
{
    if (m <= 1)
        return 0;
    uint64_t val = (uint64_t)m;
    uint64_t pr[32], ex[32];
    size_t cnt = 0;
    factor_u64(val, pr, ex, &cnt);
    seq_t res = 0;
    for (size_t j = 0; j < cnt; ++j) {
        uint64_t p = pr[j];
        uint64_t e = ex[j];
        uint64_t part = val / p;        // integer division, p divides val
        res += (seq_t)(part * e);
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t H = (seq_t)1;   /* H(0) = 1  */
    seq_t D = 0;          /* D( H(0) ) = 0  */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t Di   = deriv_of_integer(i);          /* D(i) */
        seq_t pow_i = seq_pow((seq_t)i, i);        /* i^i  */

        seq_t new_H = H * pow_i;                   /* H(i)  */
        seq_t prod  = H * Di;                      /* H(i-1)*D(i)  */
        seq_t sum   = prod + D;                   /* H(i-1)*D(i) + D(H(i-1)) */
        seq_t new_D = pow_i * sum;                 /* a(i)  */

        H = new_H;
        D = new_D;
    }

    return D;
}

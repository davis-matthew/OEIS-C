/*
 * A356486  a(n) = (n-1)! * Sum_{d|n} d^n / (d-1)!.
 * https://oeis.org/A356486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)! * Sum_{d|n} d^n / (d-1)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   factorial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n=0, but harmless */

    seq_t fact_n_minus1 = factorial(n - 1);   /* (n-1)! */
    seq_t total = 0;

    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d != 0) continue;

        /* first divisor d */
        seq_t denom1 = factorial(d - 1);          /* (d-1)! */
        seq_t ratio1 = fact_n_minus1 / denom1;    /* (n-1)! / (d-1)! */
        seq_t pow1   = pow_uint(d, n);            /* d^n */
        total += ratio1 * pow1;

        idx_t d2 = n / d;
        if (d2 != d) {
            /* second divisor d2 */
            seq_t denom2 = factorial(d2 - 1);        /* (d2-1)! */
            seq_t ratio2 = fact_n_minus1 / denom2;   /* (n-1)! / (d2-1)! */
            seq_t pow2   = pow_uint(d2, n);          /* d2^n */
            total += ratio2 * pow2;
        }
    }

    return total;
}

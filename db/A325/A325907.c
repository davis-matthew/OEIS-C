/*
 * A325907  a(n) = ( (-1)^n * Sum_{k=0..n-2} (-1)^k*10^(2^k) + 10^(2^(n-1)) -
 *          ((-1)^n+3)/2 )/3.
 * https://oeis.org/A325907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((-1)^n * Sum_{k=0..n-2} (-1)^k*10^(2^k) + 10^(2^(n-1)) -
 *            ((-1)^n+3)/2)/3
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* The sequence is defined with offset 1. */
    if (n == 0) return 0;       /* undefined for n=0; safe guard */
    if (n == 1) return 3;       /* a(1) = 3 */

    /* Compute sum_even = Σ_{k even}^{n-2} 10^(2^k)
     * and sum_odd  = Σ_{k odd }^{n-2} 10^(2^k).           */
    seq_t sum_even = 0;
    seq_t sum_odd  = 0;
    for (idx_t k = 0; k <= n - 2; ++k) {
        /* exponent = 2^k   (k < 64 is assumed for the tests) */
        idx_t exponent = 1ULL << k;
        seq_t term = pow10(exponent);
        if (k & 1)
            sum_odd += term;
        else
            sum_even += term;
    }

    /* Compute the high power 10^(2^(n-1)) */
    idx_t high_exp = 1ULL << (n - 1);
    seq_t high = pow10(high_exp);

    /* Numerator: handle parity of n to avoid negative intermediates. */
    seq_t numerator;
    if ((n & 1) == 0) {          /* n even */
        numerator = sum_even + high;
        numerator -= sum_odd;    /* sum_even + high >= sum_odd */
        numerator -= 2;
    } else {                     /* n odd */
        numerator = high - sum_even;
        numerator += sum_odd;    /* high >= sum_even */
        numerator -= 1;
    }

    return numerator / 3;
}

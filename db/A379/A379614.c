/*
 * A379614  a(n) = Sum_{k=0..n} 2^((n + k - 1)*(n - k)/2) * n! / (n - k)!. Row
 *          sums of A365638.
 * https://oeis.org/A379614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^((n + k - 1)*(n - k)/2) * n! / (n - k)!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   mul_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* e0 = (n-1)*n/2 is the exponent of the first term (k = 0) */
    idx_t e0 = (n * (n - 1)) / 2;

    /* term_0 = 2^e0 * 1 (factorial part for k=0 is 1) */
    seq_t term = pow2(e0);
    seq_t sum  = term;

    seq_t pow2k = (seq_t)1;  /* 2^k , starts with 2^0 */

    for (idx_t k = 0; k < n; ++k) {
        /* term_{k+1} = term_k * (n - k) / 2^k  (the division is exact) */
        seq_t term_div = term / pow2k;
        term = mul_small(term_div, (unsigned)(n - k));

        sum += term;

        /* advance pow2k to 2^(k+1) */
        pow2k = mul_small(pow2k, 2);
    }

    return sum;
}

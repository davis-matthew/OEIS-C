/*
 * A369677  a(n) = Product_{k=0..n} (2^k + 5^(n-k)).
 * https://oeis.org/A369677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} (2^k + 5^(n-k))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   pow2, power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/power5.h"

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t pow2 = 1;             /* 2^k  */
    seq_t pow5 = power5(n);     /* 5^(n-k) when k = 0 */

    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = pow2 + pow5;   /* 2^k + 5^(n-k) */
        result *= term;

        pow2 *= 2;     /* advance to next 2^k    */
        pow5 /= 5;     /* advance to next 5^(n-k) */
    }
    return result;
}

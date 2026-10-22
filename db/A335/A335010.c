/*
 * A335010  a(n) = Product_{k=1..n} (2^k - 1)^k.
 * https://oeis.org/A335010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=1..n} (2^k - 1)^k
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* a(1) is the first term; behaviour for n==0 is unspecified. */
    if (n == 0) {
        return 0;
    }

    seq_t result = 1;
    seq_t pow2 = 1;                   /* will hold 2^k (k starts at 0) */

    for (idx_t k = 1; k <= n; ++k) {
        pow2 *= 2;                    /* pow2 = 2^k */
        seq_t base = pow2 - 1;        /* base = 2^k - 1 */
        seq_t term = big_pow(base, (seq_t)k);
        result *= term;
    }

    return result;
}

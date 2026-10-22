/*
 * A390325  Oblong numbers of the form 2^k*(2^k - 1) or 2^k*(2^k + 1).
 * https://oeis.org/A390325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^ceil(n/2) * ( 2^ceil(n/2) + (-1)^n ) where ceil(n/2) =
 *            (n+1)/2 odd n -> a(n) = 2^k * (2^k - 1) even n -> a(n) = 2^k *
 *            (2^k + 1) with k = (n+1)/2
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* The sequence is defined for n >= 1. For n==0 return 0 for safety. */
    if (n == 0) return 0;

    idx_t k = (n + 1) >> 1;          /* ceil(n/2) */
    seq_t p = pow2(k);               /* 2^k */

    if (n & 1)                       /* odd n */
        return p * (p - 1);
    else                             /* even n */
        return p * (p + 1);
}

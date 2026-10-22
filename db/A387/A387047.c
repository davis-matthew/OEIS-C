/*
 * A387047  Number of parking functions of size n with a big descent in the
 *          first position.
 * https://oeis.org/A387047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-2)/2 * (n+1)^(n-2) for n >= 2, a(1) = 0
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* a(1) and a(2) are 0 by definition */
    if (n <= 2)
        return (seq_t)0;

    /* (n+1)^(n-2) */
    seq_t base = (seq_t)(n + 1);
    seq_t exp  = (seq_t)(n - 2);
    seq_t power = big_pow(base, exp);

    /* Compute (n-2)/2 * power accurately for all n */
    idx_t nm2 = n - 2;
    seq_t result;
    if ((nm2 & 1ULL) == 0) {          /* n-2 is even */
        result = ((seq_t)nm2 / (seq_t)2) * power;
    } else {                         /* n-2 is odd → power is even */
        result = (power / (seq_t)2) * (seq_t)nm2;
    }
    return result;
}

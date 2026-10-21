/*
 * A280368  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 259", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A280368
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

/*
 * Formula: a(0)=1, a(1)=10;
 * for n even > 0:  a(n) = 11 * 10^(n-1)
 * for n odd  > 1:  a(n) = (10^n – 100) / 900
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)10;

    if ((n & 1U) == 0) {                   /* even n   */
        seq_t p = big_pow((seq_t)10, (seq_t)(n - 1));
        return p * (seq_t)11;
    } else {                                /* odd n    */
        seq_t p = big_pow((seq_t)10, (seq_t)n);
        p = p - (seq_t)100;
        return p / (seq_t)900;
    }
}

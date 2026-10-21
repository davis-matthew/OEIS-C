/*
 * A287193  Decimal representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 253", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A287193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 1, a(2) = 0. For n > 2: if n is odd : a(n) =
 *            2^n - 1 if n is even: a(n) = 3 * 2^(n-1)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)0;

    /* compute 2^n */
    seq_t pow2 = pow_seq((seq_t)2, (int)n);

    if (n & 1) {          /* odd n */
        return pow2 - (seq_t)1;
    } else {              /* even n */
        seq_t half = pow2 >> 1;   /* 2^(n-1) */
        return half * (seq_t)3;   /* 3 * 2^(n-1) */
    }
}

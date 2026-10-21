/*
 * A266983  Binary representation of the n-th iteration of the "Rule 81"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*10^(2n-1) for even n (n>0), * a(n) = (10^(2n-1)-1)/9
 *            for odd n (n>0), * a(0) = 1 * (OEIS A266983)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* compute 10^(2n-1) */
    idx_t exp = 2*n - 1;
    seq_t pow10 = big_pow((seq_t)10, (seq_t)exp);

    if (n & 1U) {          /* odd n */
        return (pow10 - (seq_t)1) / (seq_t)9;
    } else {               /* even n */
        return pow10 * (seq_t)11;
    }
}

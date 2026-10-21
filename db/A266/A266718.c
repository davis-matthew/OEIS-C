/*
 * A266718  Decimal representation of the n-th iteration of the "Rule 59"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 If n is even (n ≥ 2) a(n) = 3 * 2^(n/2) If n is odd (n
 *            ≥ 1) a(n) = 2^(2n+1) – (2^((n+1)/2) + 1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    if ((n & (idx_t)1) == 0) {          /* even n */
        idx_t exp = n / 2;
        /* 3 * 2^exp */
        return (seq_t)3 * pow2(exp);
    } else {                            /* odd n */
        idx_t half = (n + 1) / 2;        /* (n+1)/2 */
        idx_t bigExp = n * 2 + 1;        /* 2n+1 */
        seq_t bigPow  = pow2(bigExp);
        seq_t smallPow = pow2(half);
        return bigPow - (smallPow + (seq_t)1);
    }
}

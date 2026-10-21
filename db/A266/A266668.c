/*
 * A266668  Decimal representation of the n-th iteration of the "Rule 51"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266668
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*4^n - 2^n - 1 for n odd; a(n) = 2^n for n even
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute 2^n once, it is needed for both cases            */
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);

    /* If n is even, a(n) is simply 2^n                        */
    if ((n & 1) == 0)
        return pow2;

    /* For odd n: a(n) = 2*4^n - 2^n - 1                      */
    seq_t pow4 = big_pow((seq_t)4, (seq_t)n);
    seq_t res  = pow4 + pow4;          /* 2*4^n */
    res = res - pow2;                  /* 2*4^n - 2^n */
    res = res - (seq_t)1;              /* 2*4^n - 2^n - 1 */
    return res;
}

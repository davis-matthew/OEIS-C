/*
 * A267851  Decimal representation of the n-th iteration of the "Rule 229"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n==0, 2 if n==1, 2 * (4^n - 1) otherwise (n >= 2)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;
    /* Compute 2 * (4^n - 1) */
    seq_t pow4 = big_pow((seq_t)4, (seq_t)n); /* big_pow returns 4^n */
    seq_t tmp = pow4 - (seq_t)1;
    return tmp * (seq_t)2;
}

/*
 * A267867  Binary representation of the n-th iteration of the "Rule 231"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(n) = 10 * ((10^(2n) - 1) / 9), n ≥ 1 (i.e. the
 *            decimal integer whose digits form the binary string of n
 *            iterations of Rule 231: 2n ones followed by a zero)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* exponent 2*n (fits in idx_t for n ≤ 10 000) */
    idx_t exp = n << 1;        /* 2 * n */

    seq_t pow10 = seq_pow((seq_t)10, exp);   /* 10^(2n) */
    seq_t tmp   = (pow10 - (seq_t)1) / (seq_t)9;  /* (10^(2n)-1)/9  */
    tmp *= (seq_t)10;          /* append the trailing 0 */

    return tmp;
}

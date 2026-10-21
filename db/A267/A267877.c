/*
 * A267877  Decimal representation of the n-th iteration of the "Rule 233"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=0, a(2)=21, a(3)=119, a(n)=2^(2*n+1)-1 for n>=4
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Handle the first four values explicitly */
    static const seq_t init_vals[4] = { (seq_t)1, (seq_t)0, (seq_t)21, (seq_t)119 };
    if (n < 4)
        return init_vals[n];

    /* For n >= 4 use the closed form a(n) = 2^(2*n+1) - 1 */
    idx_t exp = 2*n + 1;          /* exponent for the power of two */
    seq_t val = seq_pow((seq_t)2, exp);   /* 2^exp, exact big‑int if needed */
    return val - (seq_t)1;        /* subtract one */
}

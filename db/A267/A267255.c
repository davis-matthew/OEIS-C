/*
 * A267255  Decimal representation of the n-th iteration of the "Rule 111"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A267255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=6, a(2)=7, a(3)=122, a(4)=31; For n>4: if n is
 *            even, a(n)=95; if n is odd, a(n)=2*4^n-30 (or equivalently
 *            a(n)=17*a(n-2)-16*a(n-4) for n>8)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    static const seq_t a0[] = {1, 6, 7, 122, 31};

    if (n < 5)
        return a0[n];

    if ((n & 1) == 0)   /* even n > 4 */
        return (seq_t)95;

    /* odd n > 4: a(n) = 2*4^n - 30 */
    seq_t pow4 = big_pow((seq_t)4, (seq_t)n);    /* compute 4^n */
    seq_t val = pow4 + pow4;                     /* multiply by 2 */
    val -= (seq_t)30;
    return val;
}

/*
 * A264273  Number of (2+1) X (n+1) arrays of permutations of 0..n*3+2 with
 *          each element having index change +-(.,.) 0,0 1,0 or 1,2.
 * https://oeis.org/A264273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 256 * 5^(n-3) for n ≥ 3 Initial terms: a(1) = 9, a(2) =
 *            48, a(3) = 256
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the offset properly; behaviour below n = 1 is undefined. */
    if (n == 1)
        return (seq_t)9;
    if (n == 2)
        return (seq_t)48;
    if (n == 3)
        return (seq_t)256;

    /* For n > 3: a(n) = 256 * 5^(n-3) */
    seq_t pow5 = big_pow((seq_t)5, (seq_t)(n - 3));
    return (seq_t)256 * pow5;
}

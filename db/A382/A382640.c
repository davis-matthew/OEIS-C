/*
 * A382640  a(n) = 90*binomial(n,6) + 90*binomial(n,5) + 54*binomial(n,4) +
 *          24*binomial(n,3) + 9*binomial(n,2) + 3*n + 1.
 * https://oeis.org/A382640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 90*binomial(n,6) + 90*binomial(n,5) + 54*binomial(n,4) +
 *            24*binomial(n,3) + 9*binomial(n,2) + 3*n + 1
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 58, u64 n <= 2300,
 *            u128 n <= 10000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Computes A382640: number of words of length n over 4 symbols
   where the three largest symbols occur at most twice. */
seq_t a(idx_t n)
{
    seq_t res = 0;
    if (n >= 6) res += (seq_t)90 * binomial(n, 6);
    if (n >= 5) res += (seq_t)90 * binomial(n, 5);
    if (n >= 4) res += (seq_t)54 * binomial(n, 4);
    if (n >= 3) res += (seq_t)24 * binomial(n, 3);
    if (n >= 2) res += (seq_t)9  * binomial(n, 2);
    res += ((seq_t)3 * n) + 1;
    return res;
}

/*
 * A380445  a(n) = 10*binomial(n,5) + 6*binomial(n,4) + binomial(n,3) +
 *          binomial(n,2).
 * https://oeis.org/A380445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*binomial(n,5) + 6*binomial(n,4) + binomial(n,3) +
 *            binomial(n,2)
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 140, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t res = 0;

    res += binomial(n, 2);
    res += binomial(n, 3);
    res += 6 * binomial(n, 4);
    res += 10 * binomial(n, 5);

    return res;
}

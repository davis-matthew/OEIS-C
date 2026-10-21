/*
 * A280322  Sum of the squares of the larger parts of the partitions of 2n
 *          into two squarefree parts.
 * https://oeis.org/A280322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} (2n-i)^2 * mu(i)^2 * mu(2n-i)^2 where mu is
 *            the Möbius function. Only the terms where both i and 2n-i are
 *            square‑free contribute. The summand (2n-i)^2 is accumulated in a
 *            seq_t variable
 * Range:     u8 n <= 5, u16 n <= 35, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t mu_i = mobius(i);
        if (!mu_i) continue;                       // i not square‑free

        idx_t j = (idx_t)2 * n - i;                // 2n - i
        seq_t mu_j = mobius(j);
        if (!mu_j) continue;                       // 2n-i not square‑free

        seq_t sq = (seq_t)j * (seq_t)j;            // (2n-i)^2
        sum += sq;
    }
    return sum;
}

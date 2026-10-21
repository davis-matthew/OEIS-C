/*
 * A294245  Sum of the larger parts of the partitions of 2n into two parts
 *          with larger part nonsquarefree.
 * https://oeis.org/A294245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=n}^{2*n-1} k * (1 - mu(k)^2) where mu is Möbius
 * Range:     u8 n <= 22, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"

/* Compute the n-th term of OEIS A294245 */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n == 0) return 0;               /* offset starts at 1, but guard anyway */
    idx_t end = 2 * n;                  /* exclusive upper bound (2*n - 1 inclusive) */
    for (idx_t k = n; k < end; ++k) {
        seq_t mu = mobius(k);
        if (mu == 0) {                  /* add k only when k is not square‑free */
            sum += (seq_t)k;
        }
    }
    return sum;
}

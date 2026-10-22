/*
 * A309126  a(n) = n + 2^3 * floor(n/2^3) + 3^3 * floor(n/3^3) + 4^3 *
 *          floor(n/4^3) + ...
 * https://oeis.org/A309126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..⌊n^{1/3}⌋} k^3 · ⌊n / k^3⌋ (OEIS offset 1)
 * Range:     u8 n <= 71, u16 n <= 4479, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/int_cube_root.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t sum = 0;
    idx_t maxk = int_cube_root(n);   /* floor(cuberoot(n)) */

    for (idx_t k = 1; k <= maxk; ++k)
    {
        idx_t k3 = k * k * k;        /* k³ ≤ n, fits in idx_t */
        idx_t q  = n / k3;           /* floor(n / k³) */
        sum += (seq_t)k3 * (seq_t)q;
    }

    return sum;
}

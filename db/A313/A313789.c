/*
 * A313789  Coordination sequence Gal.6.208.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + number of indices k with 2 ≤ k ≤ n such that * k
 *            mod 12 ∈ {4,6,7,9}. For n = 0 the sequence starts at 1
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)5 * (seq_t)n;
    seq_t cnt = 0;

    if (n >= 4)
        cnt += ((seq_t)n - 4) / 12 + 1;
    if (n >= 6)
        cnt += ((seq_t)n - 6) / 12 + 1;
    if (n >= 7)
        cnt += ((seq_t)n - 7) / 12 + 1;
    if (n >= 9)
        cnt += ((seq_t)n - 9) / 12 + 1;

    return result + cnt;
}

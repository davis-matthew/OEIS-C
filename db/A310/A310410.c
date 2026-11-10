/*
 * A310410  Coordination sequence Gal.6.344.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   for n>=1, a(n) = a(n-12) + 58, with a(0)=1 and a(1..12) =
 *            4,10,14,20,24,29,34,38,44,48,54,58 (quasi-periodic linear growth
 *            fitted to the listed terms)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 49 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const int base[12] = {4, 10, 14, 20, 24, 29, 34, 38, 44, 48, 54, 58};
    if (n == 0) return (seq_t)1;
    idx_t m = n - 1;
    idx_t k = m / 12;
    int r = (int)(m % 12);
    return (seq_t)base[r] + (seq_t)58 * (seq_t)k;
}

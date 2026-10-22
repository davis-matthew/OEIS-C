/*
 * A315610  Coordination sequence Gal.5.256.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=6, a(2)=12; for n≥3, a(n) = 14 + 26*floor((n-3)/5)
 *            + inc[(n-3)%5], where inc[0]=0, inc[1]=6, inc[2]=12, inc[3]=18,
 *            inc[4]=24
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the three exceptional initial values */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)12;

    /* For n ≥ 3 use the periodic pattern of increments */
    static const seq_t inc[5] = {0, 6, 12, 18, 24};

    idx_t k = n - 3;                 /* number of steps beyond the 3rd term   */
    idx_t cycles = k / 5;            /* full 5‑term cycles                     */
    idx_t rem    = k % 5;            /* remaining steps in the current cycle   */

    /* Compute the value using 128‑bit intermediates if needed */
    seq_t result = (seq_t)14 + (seq_t)26 * (seq_t)cycles + inc[rem];
    return result;
}

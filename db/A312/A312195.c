/*
 * A312195  Coordination sequence Gal.5.67.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=4, a(2)=8; For n≥3: a(n)=13 + 48·⌊(n-3)/10⌋ +
 *            prefix[(n-3) mod 10], where the period of differences is 10 and
 *            prefix = {0,6,11,16,22,27,31,35,39,43,48}. (The periodic block
 *            {6,5,5,6,5,4,4,4,4,5} is summed by prefix.)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 4;
    if (n == 2) return 8;

    /* Periodic difference block (starting at n = 3). */
    static const seq_t pref[11] = {0, 6, 11, 16, 22, 27, 31, 35, 39, 43, 48};

    idx_t m = n - 3;                    /* number of periodic steps taken   */
    idx_t cycles = m / 10;              /* full periods                     */
    idx_t rem    = m % 10;              /* remaining part of the period     */

    /* Sum of diffs: full cycles + partial prefix */
    uint64_t sum = (uint64_t)cycles * 48 + (uint64_t)pref[rem];
    seq_t result = 13 + (seq_t)sum;
    return result;
}

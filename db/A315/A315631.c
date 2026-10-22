/*
 * A315631  Coordination sequence Gal.5.68.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n = 0 * a(n) = 6 for n = 1 * a(n) = 12 +
 *            44*⌊(n-2)/9⌋ * + (rem ≤ 5 ? 4*rem : 20 + 6*(rem-5)) * with rem =
 *            (n-2) mod 9 for n ≥ 2 * * This follows from the difference
 *            pattern: * diff(2) = 6, * then 5 times 4, 4 times 6, repeated
 *            forever
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;

    /* n ≥ 2 */
    idx_t d = n - 2;                     /* number of diffs after diff(2) */
    idx_t cycles = d / 9;                /* full (5×4 + 4×6) blocks */
    idx_t rem = d % 9;                   /* remaining diffs in the next block */

    /* base value a(2) = 12 */
    seq_t sum = (seq_t)12;
    sum += (seq_t)cycles * (seq_t)44;    /* each full block adds 44 */

    if (rem <= 5) {
        sum += (seq_t)rem * (seq_t)4;
    } else {
        sum += (seq_t)5 * (seq_t)4;          /* five 4’s */
        sum += (seq_t)(rem - 5) * (seq_t)6; /* remaining 6’s */
    }

    return sum;
}

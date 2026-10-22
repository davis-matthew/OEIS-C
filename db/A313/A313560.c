/*
 * A313560  Coordination sequence Gal.6.201.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; For n ≥ 1: a(n) = a(n-1) + 4 if n == 1 or n ≥ 3 and
 *            ((n-3) mod 11 == 0 or 3 or 6); a(n) = a(n-1) + 5 otherwise.
 *            (This reproduces the OEIS sequence A313560.)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;            /* a(0) = 1 */

    seq_t value = 1;                 /* current a(i) */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t inc = 5;               /* default increment */

        if (i == 1) {
            inc = 4;
        } else if (i >= 3) {
            idx_t r = (i - 3) % 11;   /* periodic pattern 3–3–5 (period 11) */
            if (r == 0 || r == 3 || r == 6)
                inc = 4;
        }

        value += inc;
    }

    return value;
}

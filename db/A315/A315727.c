/*
 * A315727  Coordination sequence Gal.5.89.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   For 0 ≤ n < 6: a(n) = {1, 6, 12, 18, 22, 26} For n ≥ 6: Let k =
 *            n - 6, c = k / 9 (integer division), r = k % 9. a(n) = 30 + 48*c
 *            + (r <= 6 ? 6*r : 36 + 4*(r - 6)) This implements the periodic
 *            pattern of +6 and +4 increments with a cycle of length 9
 *            starting from n = 6
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle the first six values directly. */
    if (n < 6U) {
        static const seq_t vals[6] = {1, 6, 12, 18, 22, 26};
        return vals[n];
    }

    /* Compute for n ≥ 6 using the repeating 6/6/6/6/6/6,4/4/4 pattern. */
    idx_t steps = n - 6U;          /* number of increments after a(6) */
    idx_t cycles = steps / 9U;      /* full 9‑step cycles */
    idx_t rem    = steps % 9U;      /* remaining steps within current cycle */

    idx_t increment = 48U * cycles; /* 6 increments of 6 and 3 of 4 per cycle */

    if (rem <= 6U) {
        /* First 'rem' steps are +6 each. */
        increment += 6U * rem;
    } else {
        /* All 6 steps of +6 and the remaining (rem−6) steps of +4. */
        increment += 36U + 4U * (rem - 6U);
    }

    return (seq_t)(30U + increment);
}

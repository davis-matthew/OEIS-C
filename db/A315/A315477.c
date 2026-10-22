/*
 * A315477  Coordination sequence Gal.5.293.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-5)+27 for n >= 6 Initial values: a(0) = 1, a(1) = 6,
 *            a(2) = 11, a(3) = 16, a(4) = 21, a(5) = 27
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Pre‑computed initial values for n = 0,…,5 */
    static const seq_t init[6] = {1, 6, 11, 16, 21, 27};

    if (n <= 5)                 /* directly return the known term */
        return init[n];

    /* For n >= 6 use the recurrence a(n) = a(n-5) + 27.
       This yields a closed form that depends on n % 5. */
    const seq_t mult = (seq_t)27 * (seq_t)(n / 5); /* 27 * floor(n/5) */

    if ((n % 5) == 0)           /* n is a multiple of 5 : a(5k) = 27 * k  */
        return mult;

    /* otherwise a(5k + r) = a(r) + 27 * k , 1 <= r <= 4 */
    return init[n % 5] + mult;
}

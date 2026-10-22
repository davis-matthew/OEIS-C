/*
 * A313687  Coordination sequence Gal.6.202.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313687
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-12) + 58 (for n ≥ 1), with initial values a(0) = 1,
 *            a(1) = 5, a(2) = 10, a(3) = 15, a(4) = 19, a(5) = 23, a(6) = 29,
 *            a(7) = 35, a(8) = 39, a(9) = 43, a(10) = 48, a(11) = 53, a(12) =
 *            58
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The first 12 values (indices 0 to 11).  */
    static const seq_t base[12] = {
        1,  /* a(0) */
        5,  /* a(1) */
        10, /* a(2) */
        15, /* a(3) */
        19, /* a(4) */
        23, /* a(5) */
        29, /* a(6) */
        35, /* a(7) */
        39, /* a(8) */
        43, /* a(9) */
        48, /* a(10) */
        53  /* a(11) */
    };

    if (n == 0)
        return 1;                /* a(0) */

    /* floor division and remainder with 12 */
    idx_t div12 = n / 12;
    idx_t mod12 = n % 12;

    /* For multiples of 12 the value is 58 * (n/12).  */
    if (mod12 == 0)
        return (seq_t)58 * (seq_t)div12;

    /* Non‑multiples use the pre‑stored base value plus the
       accumulated 58 per completed cycle.                    */
    return base[mod12] + (seq_t)58 * (seq_t)div12;
}

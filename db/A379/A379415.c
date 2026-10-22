/*
 * A379415  a(n) = n + floor(n*r/s) + floor(n*t/s), where r = 3^(1/4), s =
 *          3^(1/2), t = 3^(3/4).
 * https://oeis.org/A379415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n/r) + floor(n*r), where r = 3^(1/4)
 * Range:     u8 none, u16 n <= 3, u32 n <= 56, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   floor_n_root_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_n_root_factor.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safeguard for unsafe calls */

    seq_t N     = (seq_t)n;

    /* floor(n * r)  →  factor 3  (since r^4 = 3) */
    seq_t floor_nr = floor_n_root_factor(n, 3);

    /* floor(n * r^3) → factor 27 (since (r^3)^4 = 27) */
    seq_t floor_ns = floor_n_root_factor(n, 27);

    /* floor(n / r) = floor( floor(n * r^3) / 3 ) */
    seq_t floor_nr_inv = floor_ns / 3;

    return N + floor_nr + floor_nr_inv;
}

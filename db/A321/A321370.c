/*
 * A321370  n + [n*s/r] + [n*t/r], where r = 1, s = cos(2*Pi/5), t =
 *          sec(2*Pi/5).
 * https://oeis.org/A321370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n*cos(2*π/5)⌋ + ⌊n*sec(2*π/5)⌋
 * Range:     u8 n <= 3, u16 n <= 56, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent integer form: a(n) = 2*n + floor(n*√5) + (floor(n*√5) - n)/4 */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic */
    seq_t N = (seq_t)n;

    /* 5 * n^2 : safe for n up to ≈ 3·10^4 in 32‑bit builds */
    seq_t m = N * N * 5;          /* m = 5*n^2 */
    seq_t fs = floor_sqrt(m);     /* fs = floor(n*√5) */

    /* floor((floor(n*√5)-n)/4)  = (fs - N)/4  (integer division) */
    seq_t extra = (fs - N) / 4;

    /* a(n) = 2*n + fs + extra */
    return 2 * N + fs + extra;
}

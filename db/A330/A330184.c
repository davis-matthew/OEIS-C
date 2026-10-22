/*
 * A330184  a(n) = n + floor(nr/t) + floor(ns/t), where r = sqrt(2) - 1/2, s =
 *          sqrt(2), t = sqrt(2) + 1/2.
 * https://oeis.org/A330184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + ⌊n r / t⌋ + ⌊n s / t⌋, r = √2 – ½, s = √2, t = √2 + ½
 *            With integer arithmetic: r/t = (9 – 4√2)/7, s/t = (8 – 2√2)/7
 *            Hence a(n) = n + ⌊(9n – ⌊4n√2⌋ – 1)/7⌋ + ⌊(8n – ⌊2n√2⌋ – 1)/7⌋
 * Range:     u8 n <= 1, u16 n <= 22, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

static inline seq_t floor_k_sqrt2(idx_t k)
{
    /* floor(k * sqrt(2)) = floor( sqrt(2 * k^2) ) */
    __int128 prod = (__int128)k * (__int128)k * 2;
    return floor_sqrt((seq_t)prod);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* not used, offset starts at 1   */

    seq_t N = (seq_t)n;
    seq_t f4 = floor_k_sqrt2((idx_t)(4*n));
    seq_t f2 = floor_k_sqrt2((idx_t)(2*n));

    seq_t part1 = ((seq_t)9 * N - f4 - 1) / 7;
    seq_t part2 = ((seq_t)8 * N - f2 - 1) / 7;

    return N + part1 + part2;
}

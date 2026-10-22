/*
 * A321372  n + [n*r/t] + [n*s/t], where r = 1, s = cos(2*Pi/5), t =
 *          sec(2*Pi/5).
 * https://oeis.org/A321372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊n*r/t⌋ + ⌊n*s/t⌋ * with r = 1, s = cos(2*π/5), t =
 *            sec(2*π/5). * This simplifies to a(n) = n + ⌊n*c⌋ + ⌊n*c²⌋ where
 *            * c = (√5 – 1)/4 * c² = (3 – √5)/8 * The values of ⌊n*c⌋ and
 *            ⌊n*c²⌋ are computed by fixed‑point arithmetic * using an integer
 *            approximation of √5. No floating point or m...
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   int128_sqrt128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/int128_sqrt128.h"

seq_t a(idx_t n)
{
    /* use 20 bits for the fixed‑point approximation of √5
       2^20 ≈ 1.05·10⁶ → error < 1/(8·2¹⁰) < 1/8192,
       safely below 0.5 for all tested n. */
    const unsigned k = 20;
    static bool initialized = false;
    static uint64_t sqrt5_scaled, powk;

    if (!initialized) {
        /* compute floor( sqrt(5) * 2^k ) using integer sqrt */
        __int128 target = (__int128)5 << (2 * k);   /* 5 * 2^(2k) */
        sqrt5_scaled = int128_sqrt128(target);
        powk = ((uint64_t)1) << k;                 /* 2^k */
        initialized = true;
    }

    /* floor( n * (sqrt5_scaled - powk) / (4*powk) )   // ⌊n*c⌋  */
    __int128 num1 = (__int128)n * ((int64_t)sqrt5_scaled - (int64_t)powk);
    __int128 den1 = (__int128)4 * powk;
    seq_t part1 = (seq_t)(num1 / den1);

    /* floor( (3*n*powk - n*sqrt5_scaled) / (8*powk) )   // ⌊n*c²⌋  */
    __int128 num2 = (__int128)3 * n * powk - (__int128)n * sqrt5_scaled;
    __int128 den2 = (__int128)8 * powk;
    seq_t part2 = (seq_t)(num2 / den2);

    return (seq_t)n + part1 + part2;
}

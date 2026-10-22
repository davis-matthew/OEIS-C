/*
 * A347912  a(n) = Sum_{k=1..n} k - floor(sqrt(k)+1/2) * floor(sqrt(k-1)).
 * https://oeis.org/A347912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} (k - floor(sqrt(k)+1/2) * floor(sqrt(k-1)))
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the sequence value a(n) for any n >= 1. */
seq_t a(idx_t n)
{
    /* Offset 1 – behaviour for n = 0 is irrelevant */
    if (n == 0) return 0;

    /* Use 128‑bit arithmetic to avoid overflow for intermediate products. */
    unsigned __int128 total128 = (unsigned __int128)n * ((unsigned __int128)n + 1) / 2;
    unsigned __int128 sum_f128 = 0;

    idx_t root      = 0; /* floor(sqrt(k)) for current k */
    idx_t root_prev = 0; /* floor(sqrt(k-1)) */

    for (idx_t k = 1; k <= n; ++k) {
        /* Increment root until (root+1)^2 > k. */
        while (1) {
            unsigned __int128 next = (unsigned __int128)(root + 1) * (root + 1);
            if (next <= k)
                ++root;
            else
                break;
        }

        /* round_sqrt(k) = floor(sqrt(k)+1/2) */
        idx_t round = root;
        idx_t threshold = root * root + root + 1;
        if (k >= threshold) ++round;

        /* f(k) = round_sqrt(k) * floor_sqrt(k-1) */
        unsigned __int128 prod = (unsigned __int128)round * (unsigned __int128)root_prev;
        sum_f128 += prod;

        /* For the next iteration, floor_sqrt(k) becomes floor_sqrt(k-1). */
        root_prev = root;
    }

    unsigned __int128 result128 = total128 - sum_f128;
    return (seq_t)result128;
}

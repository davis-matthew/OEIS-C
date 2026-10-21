/*
 * A249298  Smallest positive integer k, such that s-k*n is a square where s
 *          is the smallest square >= k*n.
 * https://oeis.org/A249298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A249298: Find the smallest positive integer k such that
 * ceil(sqrt(k*n))^2 - k*n is a perfect square.
 * The implementation uses a brute‑force search; for each k it
 * computes the next square after k*n and checks if the
 * difference is a square.  ceils are handled via floor_sqrt. */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    idx_t k = 1;
    for (;; ++k) {
        /* x = k * n */
        seq_t x = (seq_t)k * (seq_t)n;

        /* t = ceil(sqrt(x)) */
        seq_t t = floor_sqrt(x);
        if (t * t < x) ++t;

        /* difference between the next square and x */
        seq_t diff = t * t - x;

        /* is diff a square? */
        seq_t r = floor_sqrt(diff);
        if (r * r == diff) {
            return k;
        }
    }
}

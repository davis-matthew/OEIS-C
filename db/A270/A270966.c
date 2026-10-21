/*
 * A270966  Number of ways to write n as x^2 + y^2 + z*(3z+1)/2, where x, y
 *          and z are integers with 0 <= x <= y such that x or y has the form
 *          p-1 with p prime.
 * https://oeis.org/A270966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A270966: number of triples (x,y,z) with 0 <= x <= y,
   x or y = p-1 for some prime p, satisfying n = x^2 + y^2 + z*(3z+1)/2.
   The function enumerates all integer z, then all integer solutions
   of x^2 + y^2 = n - z*(3z+1)/2, and counts those where x or y is one
   less than a prime. */
#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/is_prime.h"

/* Return a(n) for n >= 1 */
seq_t a(idx_t n)
{
    seq_t count = 0;
    /* 1. Non‑negative z (z = 0,1,2,...) */
    for (idx_t z = 0;; ++z) {
        seq_t f = (seq_t)z * (((seq_t)3 * z) + 1) / 2;  /* z*(3z+1)/2 */
        if (f > (seq_t)n) break;
        seq_t rem = (seq_t)n - f;          /* remaining to be expressed as x^2 + y^2 */

        idx_t maxx = (idx_t)floor_sqrt(rem);
        for (idx_t x = 0; x <= maxx; ++x) {
            seq_t l = rem - ((seq_t)x * x);
            idx_t y = (idx_t)floor_sqrt(l);
            if ((seq_t)y * y != l) continue;   /* not a square */
            if (y < x) continue;               /* enforce 0 <= x <= y */

            if (is_prime(x + 1) || is_prime(y + 1)) {
                ++count;
            }
        }
    }

    /* 2. Negative z (z = -k, k >= 1) */
    for (idx_t k = 1;; ++k) {
        /* z = -k : z*(3z+1)/2 = k*(3k-1)/2   (positive) */
        seq_t f = (seq_t)k * (((seq_t)3 * k) - 1) / 2;
        if (f > (seq_t)n) break;
        seq_t rem = (seq_t)n - f;

        idx_t maxx = (idx_t)floor_sqrt(rem);
        for (idx_t x = 0; x <= maxx; ++x) {
            seq_t l = rem - ((seq_t)x * x);
            idx_t y = (idx_t)floor_sqrt(l);
            if ((seq_t)y * y != l) continue;
            if (y < x) continue;

            if (is_prime(x + 1) || is_prime(y + 1)) {
                ++count;
            }
        }
    }

    return count;
}

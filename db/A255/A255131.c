/*
 * A255131  n minus the least number of squares that add up to n: a(n) = n -
 *          A002828(n).
 * https://oeis.org/A255131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - A002828(n)
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* return true if idx_t n is a perfect square */
static bool is_perfect_square(idx_t n)
{
    if (n == 0) return true;
    seq_t r = floor_sqrt((seq_t)n);
    return r * r == (seq_t)n;
}

/* return true if n can be expressed as a sum of two squares
   (using the theorem on prime factorisation). */
static bool sum_of_two_squares(idx_t n)
{
    if (n == 0) return true;
    idx_t r = n;
    for (idx_t p = 2; p <= r / p; ++p) {
        if (r % p == 0) {
            int exp = 0;
            while (r % p == 0) {
                r /= p;
                ++exp;
            }
            if ((p & 3) == 3 && (exp & 1) == 1)  /* p % 4 == 3 && exp odd */
                return false;
        }
    }
    if (r > 1 && (r & 3) == 3)  /* remaining prime factor */
        return false;
    return true;
}

/* minimal number of squares that sum to n (1‑4, or 0 for n = 0) */
static seq_t min_squares_uint64(idx_t n)
{
    if (n == 0) return 0;
    if (is_perfect_square(n)) return 1;
    if (sum_of_two_squares(n)) return 2;

    /* Legendre's three‑square theorem */
    idx_t t = n;
    while ((t & 3) == 0) t >>= 2;  /* remove factors of 4 */
    if ((t & 7) == 7) return 4;
    return 3;
}

/* OEIS sequence A255131 */
seq_t a(idx_t n)
{
    seq_t sq = min_squares_uint64(n);
    return ((seq_t)n) - sq;
}

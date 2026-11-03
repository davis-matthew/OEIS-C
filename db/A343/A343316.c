/*
 * A343316  Array T(n, k), n, k > 0, read by antidiagonals; the balanced
 *          ternary representation of T(n, k) is obtained by multiplying
 *          componentwise the digits in the balanced ternary representations
 *          of n and of k.
 * https://oeis.org/A343316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, base
 * Formula:   a(n) = T(i,j) where (i,j)=antidiagonal_coords(n); T(i,j) is
 *            defined by componentwise multiplication of the balanced‑ternary
 *            digits of i and j
 * Range:     i8 n <= 10010, i16 n <= 10010, i32 n <= 10010, i64 n <= 10010,
 *            i128 n <= 10010, big any n
 * Validated: standard, pass, 94 terms through n = 9772 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* mapping from linear index n to pair (i,j) on the antidiagonals of T */
static inline void antidiagonal_coords(idx_t n, idx_t *i, idx_t *j)
{
    /* find s such that (s+1)(s+2)/2 <= n < (s+2)(s+3)/2 */
    idx_t s = 0;
    while (((s + 1) * (s + 2)) / 2 <= n) ++s;
    idx_t pos = n - (s * (s + 1)) / 2;  /* 0 <= pos <= s */
    *i = pos;
    *j = s - pos;
}

static seq_t balanced_product(idx_t x, idx_t y)
{
    seq_t result = 0;
    seq_t pow3 = 1;

    while (x != 0 || y != 0)
    {
        int dx = x % 3;          /* balanced digits of x */
        x /= 3;
        if (dx == 2) {
            dx = -1;
            x += 1;
        }

        int dy = y % 3;          /* balanced digits of y */
        y /= 3;
        if (dy == 2) {
            dy = -1;
            y += 1;
        }

        int prod = dx * dy;      /* -1, 0, or 1 */
        result += (seq_t)prod * pow3;
        pow3 *= (seq_t)3;
    }
    return result;
}

seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);
    return balanced_product(i, j);
}

/*
 * A371767  Triangle read by rows: T(n, k) = (k! * n!)/(n - k)!.
 * https://oeis.org/A371767
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where T(r,k) = (k! * r!)/(r - k)!, r =
 *            floor((sqrt(8*n + 1) - 1)/2), k = n - r*(r+1)/2. Initial values:
 *            a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=2, a(5)=4,
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t floor_sqrt(idx_t v)
{
    if (v < 2) return v;
    idx_t x = v;
    idx_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + v / x) >> 1;
    }
    return x;
}

seq_t a(idx_t n)
{
    /* Find the row r that contains index n. */
    idx_t v = 8*n + 1;
    idx_t s = floor_sqrt(v);
    idx_t r = (s - 1) / 2;

    /* Starting index of that row. */
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    /* Compute T(r,k) = (k! * r!)/(r - k)! */
    seq_t res = 1;

    /* Multiplying the falling factorial part: r!/(r-k)! */
    for (idx_t i = 0; i < k; ++i) {
        res *= (seq_t)(r - i);
    }

    /* Multiply by k! */
    for (idx_t i = 1; i <= k; ++i) {
        res *= (seq_t)i;
    }

    return res;
}

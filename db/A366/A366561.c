/*
 * A366561  Triangle read by rows: T(n,k) = Sum_{y=1..n} Sum_{x=1..n}
 *          [GCD(f(x,y), n) = k], where f(x,y) = x^2 - y^2.
 * https://oeis.org/A366561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A366561 – triangle read by rows.
 * T(n,k) = Σ_{x=1..n} Σ_{y=1..n} [gcd(x²−y² , n) = k].
 * a(n) is the element obtained by reading T(n,k) row‑wise:
 *   a(1) = T(1,1), a(2) = T(2,1), a(3) = T(2,2), …
 * The implementation uses a direct O(r²) double loop for the
 * selected ∙row r∙ and column ∙k∙, which is fast enough for
 * n ≤ 10 000.  It handles the sign of the difference correctly
 * by using a signed 64‑bit intermediate.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"

/* Find the row r such that r*(r+1)/2 ≥ n.  */
static idx_t find_row(idx_t n)
{
    idx_t low = 1, high = 1;
    while ((high * (high + 1)) / 2 < n)
        high <<= 1;
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        if ((mid * (mid + 1)) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Compute T(r,k) directly.  */
static seq_t triangle_term(idx_t r, idx_t k)
{
    seq_t count = 0;
    seq_t sr    = (seq_t)r;          /* n in the formula */

    for (idx_t x = 1; x <= r; ++x) {
        seq_t xx = (seq_t)x * (seq_t)x;
        for (idx_t y = 1; y <= r; ++y) {
            seq_t yy = (seq_t)y * (seq_t)y;
            /* signed difference to handle abs correctly on unsigned values */
            int64_t diff_s = (int64_t)xx - (int64_t)yy;
            if (diff_s < 0)
                diff_s = -diff_s;
            seq_t diff = (seq_t)diff_s;
            if (gcd(diff, sr) == (seq_t)k)
                ++count;
        }
    }
    return count;
}

/* Main sequence function required by the harness. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* defined only for n ≥ 1 */

    idx_t r = find_row(n);
    idx_t prev = (r * (r - 1)) / 2;    /* terms before row r */
    idx_t k = n - prev;                /* column within the row */

    return triangle_term(r, k);
}

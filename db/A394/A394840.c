/*
 * A394840  Minimum number of triangular cells in a connected polyiamond
 *          enclosing an interior hole of exactly n empty triangular cells.
 * https://oeis.org/A394840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2*A067628(n) + 3 + [A067628(n) < A067628(n-1)],
 *            A067628(0) = 0
 * Range:     u8 n <= 25, u16 n <= 25, u32 n <= 25, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 25 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t A394840_floor_sqrt(seq_t x)
{
    seq_t low = 1, high = x, ans = 0;
    if (x == 0) return 0;
    while (low <= high) {
        seq_t mid = low + (high - low) / 2;
        if (mid <= x / mid) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

static seq_t A394840_ceil_sqrt(seq_t x)
{
    seq_t f = A394840_floor_sqrt(x);
    if (f * f == x) return f;
    return f + 1;
}

static seq_t A067628_a(idx_t n)
{
    seq_t six_n = (seq_t)n * 6;
    seq_t c = A394840_ceil_sqrt(six_n);
    if ((c & 1) == ((seq_t)n & 1))
        return c;
    return c + 1;
}

seq_t a(idx_t n)
{
    seq_t p = A067628_a(n);
    seq_t q = (n == 0) ? (seq_t)0 : A067628_a(n - 1);
    seq_t r = 2 * p + 3;
    if (p < q) r = r + 1;
    return r;
}

/*
 * A386823  Triangle read by rows: T(n,k) = numerator((n^2 - k^2)/(n^2 +
 *          k^2)), where 0 <= k < n.
 * https://oeis.org/A386823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac, look, tabl, changed
 * Formula:   a(n) = numerator((r^2 - k^2)/(r^2 + k^2)), where r is the
 *            smallest integer with r(r+1)/2 ≥ n (so r is the row number), and
 *            k = n - r(r-1)/2 - 1 (k is the column index, 0‑based)
 * Range:     u8 n <= 137, u16 n <= 9348, u32 n <= 9348, u64 n <= 9348,
 *            u128 n <= 9348, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses only integer arithmetic and the
   gcd helper from the utilities.  No floating‑point operations
   are used. */

#include "../utils/gcd.h"   /* seq_t gcd(seq_t a, seq_t b); */

static idx_t find_row(idx_t n)
/* Return the smallest r such that r*(r+1)/2 ≥ n. */
{
    /* For simple bounds we use a doubling and binary search. */
    idx_t low = 1, high = 1;
    /* Increase high until the condition holds. */
    while (((__int128)high * (high + 1)) / 2 < n) {
        high <<= 1;
    }
    /* Binary search between low and high. */
    low = high >> 1;
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (((__int128)mid * (mid + 1)) / 2 >= n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0) {          /* offset 1: n=0 is undefined – return 0 */
        return (seq_t)0;
    }

    /* Determine row r and column k within that row. */
    idx_t r = find_row(n);
    idx_t prev_sum = (idx_t)(((unsigned __int128)r * (r - 1)) / 2);
    idx_t k = n - prev_sum - 1;   /* 0‑based column index */

    /* Compute numerator and denominator of (r^2 - k^2)/(r^2 + k^2) */
    seq_t rr = (seq_t)r;
    seq_t kk = (seq_t)k;
    seq_t num = rr * rr - kk * kk;
    seq_t den = rr * rr + kk * kk;

    /* Reduce the fraction and return the numerator. */
    seq_t g = gcd(num, den);
    return num / g;
}

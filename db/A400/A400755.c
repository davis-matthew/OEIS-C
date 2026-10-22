/*
 * A400755  Triangle read by rows: T(n,k) = denominator(n/(n + k)), with 1 <=
 *          k <= n.
 * https://oeis.org/A400755
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac, look, tabl, new
 * Formula:   a(n) = (r + k) / gcd(r, k) where r = ceil( ( sqrt(1+8*n) - 1 ) /
 *            2 ), k = n - r*(r-1)/2
 * Range:     u8 n <= 8382, u16 n <= 9985, u32 n <= 9985, u64 n <= 9985,
 *            u128 n <= 9985, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of OEIS sequence A400755:
   Triangle read by rows, T(n,k) = denominator of n/(n+k) with 1 ≤ k ≤ n.
   The sequence is flattened row by row. */

#include <stdint.h>
#include "../utils/gcd.h"

/* Find the least integer r such that r*(r+1)/2 >= n   (row number) */
static idx_t row_for_index(idx_t n)
{
    idx_t low  = 1;
    idx_t high = 1;

    /* increase high until it satisfies the inequality */
    while ((__int128)high * (high + 1) / 2 < n) {
        high <<= 1;                /* double high */
        if (high > n)             /* avoid overflow */
            high = n;
    }

    /* binary search for minimal r */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Main function requested by the evaluation system */
seq_t a(idx_t n)
{
    /* locate row r and column k corresponding to index n */
    idx_t r = row_for_index(n);                       /* 1‑based row   */
    idx_t prev_total = (__int128)r * (r - 1) / 2;      /* elements before row r */
    idx_t k = n - prev_total;                         /* 1‑based column within the row */

    /* denominator of n/(n+k) = (r + k) / gcd(r, k) */
    seq_t numerator   = (seq_t)r + (seq_t)k;
    seq_t g = gcd((seq_t)r, (seq_t)k);
    return numerator / g;
}

/*
 * A245300  Triangle T(n,k) = (n+k)*(n+k+1)/2 + k, 0 <= k <= n, read by rows.
 * https://oeis.org/A245300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) * where r = floor((sqrt(8*n+1)-1)/2) * k = n -
 *            r*(r+1)/2 * T(n,k) = (n+k)*(n+k+1)/2 + k * (row‑major traversal
 *            of the triangle T(n,k) defined in the * OEIS entry). The
 *            function works for any build setting of * seq_t/idx_t and
 *            returns the correct value for all n >= 0
 * Range:     u8 n <= 76, u16 n <= 8000, u32 n <= 8000, u64 n <= 8000,
 *            u128 n <= 8000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t find_row(idx_t n)
{
    /* Find the largest r such that r*(r+1)/2 <= n.
       Use exponential search followed by binary search. */
    idx_t low = 0, high = 1;
    while (1) {
        __int128 t = (__int128)high * (high + 1) / 2;
        if (t > n) break;
        high <<= 1;      /* double high until the triangular number exceeds n */
    }
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if (t <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Determine row r and column k of the flattened triangle. */
    idx_t r = find_row(n);

    /* Number of terms before row r: r*(r+1)/2. */
    __int128 before = (__int128)r * (r + 1) / 2;

    idx_t k = n - (idx_t)before;

    /* Compute the triangle entry T(r,k) = (r+k)*(r+k+1)/2 + k. */
    seq_t sum = (seq_t)(r + k);
    sum = sum * (sum + 1) / 2;   /* (r+k)*(r+k+1)/2 */
    return sum + k;
}

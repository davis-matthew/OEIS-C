/*
 * A337997  Triangle read by rows, generalized Eulerian polynomials evaluated
 *          at x = 1.
 * https://oeis.org/A337997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where (r,k) are such that n = r*(r+1)/2 + k, 0 ≤ k
 *            ≤ r, and T(r,k) = r! * k^r Here r is the row index (starting at
 *            0) and k the column index (0-based)
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdint.h>
#include <stdbool.h>

/* Compute the row index r for a given linear index n
   such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2  */
static idx_t
find_row(idx_t n)
{
    idx_t low = 0, high = n, ans = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri <= n) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

/* Compute pow(base, exp) using repeated squaring.
   The type seq_t supports normal arithmetic operators. */
static seq_t
seq_pow(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t
a(idx_t n)
{
    /* Find the row r and position k within that row */
    idx_t r = find_row(n);
    /* Triangular number r*(r+1)/2 fits in 128-bit */
    __int128 pref = (__int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)pref;

    /* Compute r! */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= r; ++i)
        fact = fact * (seq_t)i;

    /* Compute k^r */
    seq_t powv = seq_pow((seq_t)k, r);

    /* Return product */
    return fact * powv;
}

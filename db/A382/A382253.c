/*
 * A382253  Triangle T(n,k) = denominator of (n+k)/(1+n*k), 0 <= k <= n >= 0,
 *          read by rows.
 * https://oeis.org/A382253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, frac
 * Formula:   a(n) = denominator of ((r + k) / (1 + r*k)) where r is the row
 *            such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 and k = n - r*(r+1)/2.
 *            This is the sequence A382253 (denominators in the multiplication
 *            table)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include <stdint.h>

/* Find the row index r for a given flattened index n.
   We need the largest r satisfying r*(r+1)/2 <= n. */
static idx_t
row_of_index(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;          /* r cannot exceed n */
    idx_t r = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        __int128 val = (__int128)mid * (mid + 1) / 2; /* safe from overflow */
        if ((idx_t)val <= n) {
            r = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Locate the row r and the position k within that row */
    idx_t r = row_of_index(n);
    __int128 prefix128 = (__int128)r * (r + 1) / 2;
    idx_t prefix = (idx_t)prefix128;   /* number of elements before row r */
    idx_t k = n - prefix;              /* column index (0 <= k <= r) */

    /* Convert to seq_t for arithmetic */
    seq_t rn = (seq_t)r;
    seq_t kn = (seq_t)k;

    seq_t num = rn + kn;
    seq_t denom = 1 + rn * kn;

    seq_t g = gcd(num, denom);
    return denom / g;
}

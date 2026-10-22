/*
 * A385849  Triangle read by rows: T(n,k) = numerator((Sum_{i=1..k}
 *          (n-i+1)^2)/(Sum_{i=1..k} i^2)), with 1 <= k <= n.
 * https://oeis.org/A385849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac, look, tabl, changed
 * Formula:   a(n) = numerator( (1 - 3k + 2k² + 6n - 6kn + 6n²) / (1 + 3k +
 *            2k²) ) where (n,k) are the row and column of the unfolded
 *            triangle (1‑based)
 * Range:     u8 n <= 41, u16 n <= 5676, u32 n <= 9697, u64 n <= 9697,
 *            u128 n <= 9697, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

/* Compute the row number (1‑based) that contains the r‑th term. */
static idx_t
row_of_idx(idx_t r)
{
    idx_t lo = 1, hi = r;
    idx_t ans = 1;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        /* mid*(mid+1)/2 is safe in unsigned __int128 */
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= r) {
            ans = mid;
            if (mid == 0) break;
            if (mid == 0) { hi = 0; break; }
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }
    return ans;
}

/* Greatest common divisor for unsigned __int128. */
static unsigned __int128
gcd128(unsigned __int128 a, unsigned __int128 b)
{
    while (b != 0) {
        unsigned __int128 t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t
a(idx_t r)
{
    /* Find row n and column k (both 1‑based). */
    idx_t n = row_of_idx(r);                 /* row */
    unsigned __int128 before = (unsigned __int128)(n - 1) * n / 2;
    idx_t k = r - (idx_t)before;             /* column */

    /* Convert to larger integer type for computation. */
    unsigned __int128 sn = (unsigned __int128)n;
    unsigned __int128 sk = (unsigned __int128)k;

    /* Numerator: 1 - 3k + 2k² + 6n - 6kn + 6n² */
    unsigned __int128 num = 1
                           - 3 * sk
                           + 2 * sk * sk
                           + 6 * sn
                           - 6 * sk * sn
                           + 6 * sn * sn;

    /* Denominator: 1 + 3k + 2k² */
    unsigned __int128 den = 1
                           + 3 * sk
                           + 2 * sk * sk;

    /* Reduce the fraction. */
    unsigned __int128 g = gcd128(num, den);
    unsigned __int128 res = num / g;

    /* Return the result as seq_t. */
    return (seq_t)res;
}

/*
 * A339033  Triangle read by rows, T(n, k) for 0 <= k <= n. T(n, 0) = 0^n;
 *          T(n, n) = n!; otherwise T(n, k) = (n + 1 - k)*(k - 1)!.
 * https://oeis.org/A339033
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8n+1)-1)/2), k = n -
 *            r(r+1)/2. T(r,0) = 0^r (1 when r=0, otherwise 0), T(r,r) = r!,
 *            otherwise T(r,k) = (r+1-k)*(k-1)!
 * Range:     u8 n <= 26, u16 n <= 53, u32 n <= 103, u64 n <= 251,
 *            u128 n <= 664, big any n
 * Validated: standard, pass, 82 terms through n = 9495 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

seq_t a(idx_t n) {
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    idx_t cum = 0;
    while (cum + (r + 1) <= n) {
        cum += r + 1;
        r++;
    }
    idx_t k = n - cum;

    /* Handle boundary cases */
    if (k == 0) {                /* first element of the row */
        return (r == 0) ? (seq_t)1 : (seq_t)0;
    }
    if (k == r) {                /* last element of the row */
        return factorial(r);
    }

    /* Middle elements: (r+1-k)*(k-1)! */
    seq_t fac = factorial(k - 1);
    seq_t factor = (seq_t)(r + 1 - k);
    return factor * fac;
}

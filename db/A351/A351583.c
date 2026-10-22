/*
 * A351583  Triangle read by rows: T(n,k) = A(k,n-k), 1 <= k < n, 2 <= n,
 *          where A(m,n) is the number of distinct strings consisting of one
 *          X, 2*m-1 Y's and 2*n-1 Z's in which the X lies to the right of at
 *          least m Y's and at least n Z's.
 * https://oeis.org/A351583
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 32, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   binomial, count_till
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived from the OEIS formula:
   T(n,k) = (n-k)/2 * ( 2*binomial(2*n-1, 2*k-1) - binomial(n,k)*binomial(n-1,k-1) )
   The sequence a(n) is the list of T(r,k) read by rows:
   r = minimal integer with r*(r-1)/2 > n-2,
   k = (n-2) - (r-1)*(r-2)/2 + 1                                   
*/

#include "../utils/binomial.h"
#include "../utils/count_till.h"

seq_t a(idx_t n) {
    if (n < 2) return (seq_t)0;

    idx_t p = n - 2;                 /* zero‑based position in the triangle */

    /* find the row r such that count_till(r-1) <= p < count_till(r) */
    idx_t low  = 2;
    idx_t high = 2;
    while (count_till(high) <= p) high <<= 1;        /* exponential increase */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (count_till(mid) > p) high = mid;
        else                      low = mid + 1;
    }
    idx_t r = low;
    idx_t prev = count_till(r - 1);
    idx_t k = p - prev + 1;        /* 1 <= k < r */

    /* compute T(r,k) */
    seq_t c = binomial(2 * r - 1, 2 * k - 1);
    seq_t d = binomial(r, k);
    seq_t e = binomial(r - 1, k - 1);

    seq_t inner = (c * (seq_t)2) - (d * e);   /* 2*c - d*e */
    seq_t nk    = (r - k);
    seq_t res   = (inner * nk) / (seq_t)2;    /* (n-k)/2 * inner */

    return res;
}

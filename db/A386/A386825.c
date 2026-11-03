/*
 * A386825  Triangle read by rows: T(n,k) = 3^(n-k)*C(2*n,n-k).
 * https://oeis.org/A386825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 46 (2026-11-01)
 * Helpers:   binomial, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A386825
   Triangle T(n,k) = 3^(n-k) * binomial(2*n,n-k)
   Offset 0.  The sequence a(n) is the triangle entries read along rows
   (row 0 first, then row 1 left to right, etc.).
   For a given index n, let
       r = minimal integer with (r+1)*(r+2)/2 > n   (row number)
       k = n - r*(r+1)/2                           (column within the row)
   The term is a(n) = 3^(r-k) * binomial(2*r, r-k). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/pow3.h"

/* Return the row index r (0‑based) that contains position n. */
static inline idx_t _row_from_index(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t low = 0;
    idx_t high = 1;

    /* Find an upper bound where (high+1)*(high+2)/2 > n */
    while (( (__int128)(high + 1) * (high + 2) / 2 ) <= n)
        high <<= 1;

    /* Binary search for the minimal r with f(r) > n */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if (( (__int128)(mid + 1) * (mid + 2) / 2 ) <= n)
            low = mid;
        else
            high = mid;
    }
    return high;
}

seq_t a(idx_t n)
{
    /* Determine the row (r) and position within the row (k). */
    idx_t r = _row_from_index(n);
    idx_t prev = ( (__int128)r * (r + 1) ) / 2;   /* cumulative count up to row r-1 */
    idx_t k = n - prev;                           /* column index within the row */

    idx_t exp = r - k;                            /* exponent for 3 */
    idx_t bin_n = r << 1;                         /* 2*r */
    idx_t bin_k = exp;                            /* r-k */

    /* Compute T(r,k) = 3^exp * binomial(2*r, r-k) */
    seq_t three = pow3(exp);
    seq_t bin = binomial(bin_n, bin_k);
    return three * bin;
}

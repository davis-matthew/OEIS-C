/*
 * A320824  T(n, k) = (m*n)!/(k!*(n-k)!)^m with m = 3; triangle read by rows,
 *          0 <= k <= n.
 * https://oeis.org/A320824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 22, u64 n <= 58, u128 n <= 195,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A320824: triangle T(r,k) = ((3r)!/(r!)^3) * binom(r,k)^3,
   read by rows (r = 0,1,2,...).  For a given index n this function
   returns the n‑th element in that row‑major enumeration.
*/
#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Elements before row r */
    idx_t prev = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - prev;   /* column within this row */

    /* Compute the value: binom(3r,r) * binom(2r,r) * binom(r,k)^3 */
    seq_t base = binomial(3 * r, r) * binomial(2 * r, r);
    seq_t bk   = binomial(r, k);
    seq_t bk3  = bk * bk * bk;

    return base * bk3;
}

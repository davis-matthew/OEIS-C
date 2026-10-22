/*
 * A398373  Triangle read by rows: T(n, k) = binomial(n + k - 1, k) * n^(n -
 *          k).
 * https://oeis.org/A398373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 54, u64 n <= 135, u128 n <= 377,
 *            big any n
 * Helpers:   _floor_isqrt_u64, big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A398373: Triangle read by rows
   T(n,k) = binomial(n+k-1, k) * n^(n-k)
   The sequence a(n) is the flattened triangle: a(0)=T(0,0)=1,
   a(1)=T(1,0)=1, a(2)=T(1,1)=1, a(3)=T(2,0)=4, etc.
   For a given index n we compute the row r and column c:
        r = floor((sqrt(1+8n)-1)/2)
        c = n - r*(r+1)/2
   Then T(r,c) is obtained via:
        T(r,0) = r^r
        T(r,k+1) = T(r,k) * (r + k) / (r * (k+1))
*/

#include <stdint.h>
#include <assert.h>
#include "../utils/big_pow.h"   /* provides seq_t big_pow(seq_t base, seq_t exp) */
#include "../utils/_floor_isqrt_u64.h" /* provides idx_t _floor_isqrt_u64(idx_t n) */

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t disc   = 1 + 8 * n;                     /* 1+8n fits in 64‑bit for all n used */
    idx_t sqrt_d = _floor_isqrt_u64(disc);       /* integer sqrt */
    idx_t r      = (sqrt_d - 1) / 2;              /* floor((sqrt(1+8n)-1)/2) */

    /* Offset within the row */
    idx_t prev = r * (r + 1) / 2;                 /* terms before this row */
    idx_t c    = n - prev;                        /* column index */

    /* T(r,0) = r^r (with 0^0 treated as 1) */
    seq_t val = big_pow((seq_t)r, (seq_t)r);

    /* Recurrence to reach column c */
    for (idx_t k = 0; k < c; ++k) {
        seq_t numer   = (seq_t)(r + k);
        seq_t denom   = (seq_t)r * (seq_t)(k + 1);
        val = val * numer;
        val = val / denom;
    }

    return val;
}

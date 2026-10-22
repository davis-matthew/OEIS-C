/*
 * A349269  Triangle read by rows, T(n, k) = (n - k)! * k! / floor(k / 2)! ^
 *          2.
 * https://oeis.org/A349269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, k) with r = floor((sqrt(8*n+1)-1)/2) and k = n -
 *            r*(r+1)/2 where T(r, k) = (r-k)! * k! / floor(k/2)!^2
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   _floor_isqrt_u64, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factorial.h"
#include "../utils/_floor_isqrt_u64.h"  // helper to compute integer sqrt

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t const sq = _floor_isqrt_u64(8*n + 1);
    idx_t const r  = (sq - 1) / 2;

    /* Position k within the row */
    idx_t const prev = r*(r+1)/2;
    idx_t const k    = n - prev;

    /* Compute factorial parts */
    seq_t const fk      = factorial(k);
    seq_t const fl      = factorial(k/2);
    seq_t const f_r_k   = factorial(r - k);

    /* T(r, k) = (r-k)! * k! / (floor(k/2)!)^2 */
    seq_t const denom   = fl * fl;
    seq_t const numer   = f_r_k * fk;
    return numer / denom;
}

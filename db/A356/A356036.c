/*
 * A356036  Triangle read by rows, giving in the first column the powers of 3
 *          (A000244) and in the next columns 4/3 times the previous row
 *          entry.
 * https://oeis.org/A356036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 3^(r-k) * 4^k where r and k satisfy n = r*(r+1)/2 + k, 0
 *            <= k <= r (triangle read row‑wise; first column is powers of 3)
 * Range:     u8 n <= 13, u16 n <= 43, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   pow3, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"
#include "../utils/pow4.h"

seq_t a(idx_t n)
{
    /* Find row r such that (r+1)*(r+2)/2 > n
       and r*(r+1)/2 <= n.  r can be at most sqrt(2n),
       for n <= 10^4 this loop is negligible. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Index within the found row */
    idx_t prev = r * (r + 1) / 2;   /* total terms before row r */
    idx_t k = n - prev;             /* position in row r (0‑based) */

    /* Compute T(r,k) = 3^(r-k) * 4^k */
    seq_t val = pow3(r - k);
    val *= pow4(k);
    return val;
}

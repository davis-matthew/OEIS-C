/*
 * A391046  Triangle read by rows: T(n,k) = binomial(n+1,k+1) *
 *          binomial(3*n-2*k+1,k) / (n+1), 0<=k<=n.
 * https://oeis.org/A391046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, tabl, nonn
 * Formula:   T(n,k) = binomial(n+1,k+1) * binomial(3*n-2*k+1,k) / (n+1) *
 *            a(n) : the triangle read by rows, so a(n) = T(r,k) where * r =
 *            floor((sqrt(8*n+1)-1)/2) and k = n - r*(r+1)/2
 * Range:     u8 n <= 17, u16 n <= 48, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while (n >= r * (r + 1) / 2) {
        r++;
    }
    r--;                       /* r now satisfies r*(r+1)/2 <= n */
    idx_t k = n - r * (r + 1) / 2;

    idx_t n1 = r + 1;
    idx_t k1 = k + 1;
    idx_t m  = 3 * r - 2 * k + 1;

    seq_t first  = binom(n1, k1);
    seq_t second = binom(m, k);

    return (first * second) / (seq_t)n1;
}

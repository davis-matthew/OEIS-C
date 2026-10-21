/*
 * A271316  Triangle of numbers where T(n,k) is the number of k-dimensional
 *          faces on a partially truncated n-cube, 0 <= k <= n.
 * https://oeis.org/A271316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficient of x^k in (x+2)^r +
 *            2^r*(x+1)*((x+1)^(r-1)-1)/x, where r is the unique integer with
 *            r*(r+1)/2 ≤ n < (r+1)*(r+2)/2, and k = n - r*(r+1)/2. T(r,k) =
 *            C(r,k)*2^(r-k) + 2^r*( [k≥1]*C(r-1,k) + [k≤r-2]*C(r-1,k+1) )
 * Range:     u8 n <= 15, u16 n <= 47, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial, int_sqrt, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/int_sqrt.h"
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                 /* first entry 1 */

    /* Find row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t r;
    {
        idx_t t = 8 * n + 1;
        idx_t sr = int_sqrt(t);          /* floor sqrt */
        r = (sr - 1) / 2;

        /* Correct possible off by one due to rounding */
        while (( (r + 1) * (r + 2) ) / 2 <= n) r++;
        while ( (r * (r + 1)) / 2 > n) r--;
    }

    /* Column index within the row */
    idx_t k = n - (r * (r + 1)) / 2;

    /* Compute T(r,k) according to the derived formula */
    seq_t val = binomial(r, k);                 /* C(r,k) */
    val = val * pow2(r - k);                    /* * 2^(r-k) */

    seq_t two_r = pow2(r);                      /* 2^r */

    if (k >= 1) {
        seq_t add = binomial(r - 1, k) * two_r; /* 2^r * C(r-1,k) */
        val = val + add;
    }
    if (k + 1 <= r - 1) {                      /* k ≤ r-2 */
        seq_t add = binomial(r - 1, k + 1) * two_r; /* 2^r * C(r-1,k+1) */
        val = val + add;
    }

    return val;
}

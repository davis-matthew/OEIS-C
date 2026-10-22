/*
 * A391045  Triangle read by rows: T(n,k) =
 *          binomial(n+1,k+1)*binomial(2*n-k+1,k)/(n+1), 0<=k<=n.
 * https://oeis.org/A391045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, tabl, nonn
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(1+8*n) - 1) / 2) k = n -
 *            r*(r+1)/2 T(r,k) = binomial(r+1, k+1) * binomial(2*r - k + 1, k)
 *            / (r+1)
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function uses the provided binomial helper (../utils/binomial.h). */

#include <math.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2  */
    long double val = sqrtl((long double)1 + (long double)8 * (long double)n);
    idx_t r = (idx_t)((val - 1.0L) * 0.5L);  /* floor */

    /* Starting index of row r */
    idx_t start = r * (r + 1) / 2;

    /* Column index inside the row */
    idx_t k = n - start;

    /* Compute T(r,k) using the formula */
    seq_t part1 = binomial(r + 1, k + 1);
    seq_t part2 = binomial(2 * r - k + 1, k);
    seq_t result = (part1 * part2) / (r + 1);

    return result;
}

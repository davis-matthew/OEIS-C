/*
 * A347667  Triangle read by rows: T(n,k) = Sum_{j=0..k} binomial(n,j) * j! (0
 *          <= k <= n).
 * https://oeis.org/A347667
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r and k satisfy r*(r+1)/2 <= n <
 *            (r+1)*(r+2)/2 and k = n - r*(r+1)/2; T(r,k) = sum_{j=0..k}
 *            C(r,j) * j!
 * Range:     u8 n <= 19, u16 n <= 42, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;

    /* Compute the starting index of row r */
    idx_t start = r * (r + 1) / 2;
    /* Column index within the row */
    idx_t k = n - start;

    /* Sum binomial(r, j) * j! for j = 0 .. k */
    seq_t sum = 0;
    for (idx_t j = 0; j <= k; ++j) {
        seq_t bin  = binomial(r, j);
        seq_t fact = factorial(j);
        sum += bin * fact;
    }
    return sum;
}

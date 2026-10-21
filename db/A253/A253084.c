/*
 * A253084  Triangle read by rows: T(n,k) = {binomial(n+k,n-k)*binomial(n,k)}
 *          mod 2, 0 <= k <= n.
 * https://oeis.org/A253084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(r+k, r-k) * binomial(r, k) (mod 2) where r =
 *            floor((sqrt(8*n + 1) – 1) / 2) k = n – r*(r+1)/2 This is the
 *            flattened triangle read by rows of T(n,k) = { binomial(n+k, n-k)
 *            * binomial(n, k) } mod 2
 * Range:     u8 n <= 9895, u16 n <= 9895, u32 n <= 9895, u64 n <= 9895,
 *            u128 n <= 9895, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

static idx_t row_of_index(idx_t n)
{
    /* Estimate row via floating point sqrt, then correct */
    double d = sqrt((double)8.0 * n + 1.0);
    idx_t r = (idx_t)((d - 1.0) / 2.0);   /* floor */

    /* Correction: move r until r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    while (( (__int128)(r + 1) * (r + 2) / 2 ) <= n)
        r++;                                    /* bump up */
    while (( (__int128)r * (r + 1) / 2 ) > n)
        r--;                                    /* step down */
    return r;
}

seq_t a(idx_t n)
{
    idx_t r = row_of_index(n);
    idx_t start = (__int128)r * (r + 1) / 2;      /* triangular number r*(r+1)/2 */
    idx_t k = n - start;

    /* Check the bitwise condition from the OEIS entry */
    if (( (r - k) & ~(r + k) ) == 0 && ( k & ~r ) == 0)
        return (seq_t)1;
    else
        return (seq_t)0;
}

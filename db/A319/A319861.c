/*
 * A319861  Triangle read by rows, 0 <= k <= n: T(n,k) is the numerator of the
 *          k-th Bernstein basis polynomial of degree n evaluated at the
 *          interval midpoint t = 1/2; denominator is A319862.
 * https://oeis.org/A319861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl, frac
 * Formula:   a(n) is the nth entry of the flattened triangle of numerators of
 *            the Bernstein basis polynomials at t = 1/2. If n is written as n
 *            = r(r+1)/2 + k with 0 ≤ k ≤ r, then a(n) = odd_part( C(r,k) ),
 *            where C(r,k) = binomial(r,k) and odd_part(x) removes all factors
 *            of 2
 * Range:     u8 n <= 39, u16 n <= 237, u32 n <= 616, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   binom, odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/odd_part.h"

seq_t a(idx_t n)
{
    idx_t r   = 0;      /* current row index      */
    idx_t sum = 0;      /* elements before row r  */

    /* Find the row r that contains index n.  Each row r has r+1 elements. */
    while (sum + (r + 1) <= n) {
        sum += r + 1;
        r++;
    }

    /* Column index within the found row. */
    idx_t k = n - sum;

    /* Binomial coefficient and its odd part. */
    seq_t val = binom(r, k);
    return odd_part(val);
}

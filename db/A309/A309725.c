/*
 * A309725  Number of set partitions of {1,2,...,3n} with sizes in {[n, n, n],
 *          [2n, n], [3n]}.
 * https://oeis.org/A309725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3n)!/(n!^3*6) + C(3n,n) + 1, for n > 0; a(0)=3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   binom, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"   /* binom(idx_t a, idx_t b) */
#include "../utils/gcd.h"     /* gcd(seq_t a, seq_t b) */

seq_t a(idx_t n)
{
    /* Special case for n = 0 as given by the b-file */
    if (n == 0) return (seq_t)3;

    idx_t three_n = 3 * n;
    idx_t two_n   = 2 * n;

    /* Binomial coefficients */
    seq_t c3n_n = binom(three_n, n);  /* C(3n, n)   */
    seq_t c2n_n = binom(two_n,   n);  /* C(2n, n)   */

    /* Compute (c3n_n * c2n_n) / 6 without overflowing:
       cancel all factors of 6 from the product first. */
    seq_t g1 = gcd(c3n_n, (seq_t)6);
    seq_t t1 = c3n_n / g1;
    seq_t rem = (seq_t)6 / g1;

    seq_t g2 = gcd(c2n_n, rem);
    seq_t t2 = c2n_n / g2;
    rem /= g2;   /* rem should now be 1 */

    seq_t part1 = t1 * t2;          /* = (C(3n,n)*C(2n,n))/6 */

    seq_t result = part1 + c3n_n + (seq_t)1;   /* add C(3n,n) and 1 */
    return result;
}

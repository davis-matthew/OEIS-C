/*
 * A331419  a(n) is the number of subsets of {1..n} that contain exactly 4 odd
 *          numbers.
 * https://oeis.org/A331419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial((n+1)/2, 4) * 2^((n-1)/2) for odd n; * a(n) =
 *            binomial((n/2), 4) * 2^(n/2) for even n
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 39, u64 n <= 93, u128 n <= 211,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t N, exp;
    seq_t bin;

    if (n & 1) {          /* odd n */
        N   = (n + 1) / 2;
        bin = binomial(N, 4);
        exp = (n - 1) / 2;
    } else {              /* even n */
        N   = n / 2;
        bin = binomial(N, 4);
        exp = n / 2;
    }

    return bin * pow2(exp);
}

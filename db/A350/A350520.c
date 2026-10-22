/*
 * A350520  The number of degree-n^2 polynomials over Z/2Z that can be written
 *          as f(f(x)) where f is a polynomial.
 * https://oeis.org/A350520
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(0) = 1, a(1) = 1; for n >= 2: if n is even: a(n) = 2^(2*(n/2))
 *            – 2^((n/2)–1) if n is odd : a(n) = 2^(n) (since n =
 *            2·((n−1)/2)+1) which matches the conjecture a(2n)=4^n−2^(n−1)
 *            and a(2n+1)=2^(2n+1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)           /* a(0)=a(1)=1 */
        return (seq_t)1;

    if ((n & 1) == 0) {             /* even n = 2m, m >= 1 */
        idx_t m = n >> 1;           /* m = n/2 */
        return pow2(2 * m) - pow2(m - 1);
    } else {                        /* odd n = 2k+1, k >= 1 */
        idx_t k = (n - 1) >> 1;     /* k = (n-1)/2 */
        return pow2(2 * k + 1);
    }
}

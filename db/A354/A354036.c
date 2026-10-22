/*
 * A354036  a(n) = 1 if n is odd and sigma(n^2) == 1 (mod 4), otherwise 0.
 * https://oeis.org/A354036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is odd and σ(n²) ≡ 1 (mod 4); otherwise 0
 * Range:     u8 n <= 9256, u16 n <= 9256, u32 n <= 9256, u64 n <= 9256,
 *            u128 n <= 9256, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Even n cannot satisfy the condition. */
    if ((n & 1) == 0)
        return (seq_t)0;

    /* Factorise n.  We only need the prime factors of odd n. */
    idx_t m = n;
    seq_t mod4 = 1;               /* cumulative product modulo 4 */

    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            /* Odd prime p.  The contribution to σ(p^{2e}) modulo 4 is
               • 1 if p ≡ 3 (mod 4)   (independent of e)
               • (2e+1) mod 4 if p ≡ 1 (mod 4)             */
            if (p % 4 == 1) {
                seq_t factor = (seq_t)((2 * e + 1) & 3);   /* (2e+1) % 4 */
                mod4 = (seq_t)((mod4 * factor) & 3);       /* (mod4 * factor) % 4 */
            }
            /* If p ≡ 3 (mod 4) the factor is 1, so we do nothing.          */
        }
    }

    /* Remaining prime factor > sqrt(original n).    */
    if (m > 1) {                    /* m is a prime; its exponent e = 1   */
        if (m % 4 == 1) {
            seq_t factor = 3;       /* (2*1+1) % 4 = 3                     */
            mod4 = (seq_t)((mod4 * factor) & 3);
        }
        /* m ≡ 3 (mod 4) : factor is 1, no change                */
    }

    /* Return 1 iff the product is ≡ 1 (mod 4).                         */
    return (mod4 == 1) ? (seq_t)1 : (seq_t)0;
}

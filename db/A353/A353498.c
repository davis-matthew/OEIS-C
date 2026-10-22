/*
 * A353498  a(n) = 1 if n > 1 and the 2-adic valuation of phi(n) does not
 *          exceed the 2-adic valuation of n-1, otherwise 0.
 * https://oeis.org/A353498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n>1 and v₂(φ(n)) ≤ v₂(n−1), otherwise 0; a(1)=0
 *            where φ is Euler’s totient function and v₂(x) is the exponent of
 *            2 in x
 * Range:     u8 n <= 737, u16 n <= 8743, u32 n <= 8743, u64 n <= 8743,
 *            u128 n <= 8743, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n)
{
    /* a(1) is defined to be 0 */
    if (n <= 1)
        return 0;

    /* φ(n) as seq_t */
    seq_t phi_val = phi(n);

    /* compute exponent of 2 in φ(n) */
    unsigned int v2phi = 0;
    while ((phi_val & 1) == 0)
    {
        phi_val >>= 1;
        ++v2phi;
    }

    /* compute exponent of 2 in n-1 */
    idx_t m = n - 1;
    unsigned int v2n = 0;
    while ((m & 1) == 0)
    {
        m >>= 1;
        ++v2n;
    }

    return (v2phi <= v2n) ? (seq_t)1 : (seq_t)0;
}

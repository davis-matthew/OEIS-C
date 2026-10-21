/*
 * A242438  a(n) is the result of factoring a(n-1) + 1 into primes, replacing
 *          each prime 2 with a 3, and taking the product of the resulting
 *          factors.
 * https://oeis.org/A242438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 For n > 1 : a(n) = (a(n-1)+1) * (3/2)^{v_2(a(n-1)+1)}
 *            where v_2(m) is the exponent of 2 in m. The product with
 *            (3/2)^{v_2} is computed as a(n) = (a(n-1)+1) * 3^{v_2} / 2^{v_2}
 *            A small helper uses pow3() and pow2() for computing the powers
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 19, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

/* External linkage as required by the specifications */
seq_t a(idx_t n)
{
    /* Offset 1: a(1) = 1 */
    if (n < 1) {
        return (seq_t)0;  /* Undefined for n < 1, but return 0 */
    }

    seq_t cur = 1;  /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t m     = cur + 1;   /* m = a(i-1)+1 */
        seq_t tmp   = m;
        idx_t   k   = 0;         /* exponent of 2 in m */

        /* Count trailing zeros (v_2(m)) */
        while ((tmp & (seq_t)1) == 0) {
            tmp  >>= 1;
            ++k;
        }

        /* Compute a(i) = m * 3^k / 2^k */
        seq_t num = m * pow3(k);
        seq_t den = pow2(k);
        cur = num / den;
    }

    return cur;
}

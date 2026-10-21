/*
 * A283362  a(n) = (floor(2*n/3))! mod (2n-1).
 * https://oeis.org/A283362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (floor(2n/3))! mod (2n-1)
 * Range:     u8 n <= 165, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is 1‑based; return 0 for n==0 for safety */
    if (n == 0)
        return 0;

    idx_t k   = (2 * n) / 3;          /* floor(2n/3) */
    seq_t mod = (seq_t)(2 * n - 1);   /* modulus 2n-1 */

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* (res * i) cannot overflow because
           res < mod ≤ 2n-1 and i ≤ k ≤ floor(2n/3),
           so the product stays < (2n-1)*(2n/3) < 2⁶⁰ for the
           supported n range. */
        res = (res * (seq_t)i) % mod;
    }

    return res % mod;   /* ensures a(1)=0 (since 0! = 1, 1 mod 1 = 0) */
}

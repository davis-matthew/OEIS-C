/*
 * A368980  The number of exponential divisors of n that are squares
 *          (A000290).
 * https://oeis.org/A368980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 0 if n is not a perfect square; otherwise * a(n) = ∏_{p^e
 *            || √n} τ(e), where τ(k) is the divisor function * (the number of
 *            divisors of k)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   tau_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/tau_uint32.h"

/* Integer square root: largest u with u*u <= n */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t x = 0;
    idx_t bit = ((idx_t)1) << 62;   /* The greatest power of four <= n */
    while (bit > n) bit >>= 2;
    while (bit != 0) {
        if (n >= x + bit) {
            n -= x + bit;
            x = (x >> 1) + bit;
        } else {
            x >>= 1;
        }
        bit >>= 2;
    }
    return x;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but guard against 0 */

    idx_t m = isqrt_u64(n);
    if (m * m != n)                /* n not a perfect square */
        return 0;

    seq_t result = 1;
    idx_t tmp = m;

    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            unsigned int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            result *= tau_uint32(e);
        }
    }
    /* If tmp > 1, it is a prime factor with exponent 1.
       τ(1) = 1, so it does not change the product.    */

    return result;
}

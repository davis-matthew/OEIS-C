/*
 * A334768  Self-convolution of A061397.
 * https://oeis.org/A334768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n-1} P(k)*P(n-k) where P(k) = k if k is prime,
 *            0 otherwise
 * Range:     u8 n <= 17, u16 n <= 143, u32 n <= 7349, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;

    /* The sum is over k = 1 .. n-1, but only k >= 2 and n-k >= 2 can contribute,
       since P(k) = 0 for k < 2.  We iterate from 2 to n-2 inclusive. */
    for (idx_t k = 2; k + 2 <= n; ++k) {
        if (is_prime(k) && is_prime(n - k)) {
            /* cast k and (n-k) to seq_t before multiplication to avoid
               overflow in the case of very large seq_t types. */
            sum += (seq_t)k * (seq_t)(n - k);
        }
    }

    return sum;
}

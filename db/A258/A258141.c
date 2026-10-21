/*
 * A258141  Number of ways to write n as p^2 + q with p, q and 2*p + 3 all
 *          prime.
 * https://oeis.org/A258141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Computes a(n) by searching all primes p ≤ √n such that p, q = n
 *            – p², and 2p+3 are all prime. The function counts the number of
 *            such representations
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t p = 2; p <= n / p; ++p) {
        if (!is_prime(p))                    /* p must be prime   */
            continue;
        if (!is_prime(2 * p + 3))            /* 2p+3 must be prime*/
            continue;
        idx_t q = n - p * p;                 /* remaining part    */
        if (q >= 2 && is_prime(q))           /* q must be prime   */
            ++count;
    }
    return count;
}

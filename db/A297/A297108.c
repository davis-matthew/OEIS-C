/*
 * A297108  If n is prime(k)^e, e >= 1, then a(n) = 2^(k-1), otherwise 0;
 *          Möbius transform of A048675.
 * https://oeis.org/A297108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = if n = p^e (prime power) then 2^(π(p)-1) else 0 where
 *            π(p) is the number of primes ≤ p
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 136, u64 n <= 312,
 *            u128 n <= 726, big any n
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    if (n <= 1)                     /* a(1) = 0 */
        return 0;

    /* Find the smallest prime factor of n. */
    uint64_t spf = 0;
    for (uint64_t d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            spf = d;
            break;
        }
    }
    if (spf == 0)                     /* n is prime */
        spf = n;

    /* Verify that n consists only of this prime factor. */
    uint64_t t = n;
    while (t % spf == 0)
        t /= spf;
    if (t != 1)                       /* more than one distinct prime factor */
        return 0;

    /* n is a prime power p^e with p = spf.   Compute π(p). */
    seq_t pi_val = prime_pi((idx_t)spf);
    idx_t k = (idx_t)pi_val;
    if (k == 0)                       /* should not happen for spf ≥ 2 */
        return 0;

    /* Return 2^(k-1).  Multiply to avoid shift overflow. */
    seq_t res = 1;
    for (idx_t i = 0; i < k - 1; ++i)
        res *= 2;

    return res;
}

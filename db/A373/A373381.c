/*
 * A373381  a(n) = gcd(bigomega(n), A056239(n)), where bigomega is number of
 *          prime factors with repetition, and A056239 is fully additive with
 *          a(p) = primepi(p).
 * https://oeis.org/A373381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A001222(n), A056239(n))
 * Range:     u8 n <= 9617, u16 n <= 9617, u32 n <= 9617, u64 n <= 9617,
 *            u128 n <= 9617, big any n
 * Helpers:   bigomega, compute_pi, gcd, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/bigomega.h"
#include "../utils/gcd.h"
#include "../utils/smallest_prime_factor.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    /* a(1) = 0 by definition; for n >= 1 the following holds */
    if (n < 1) return 0;   /* defensive guard for n == 0 */

    /* A001222(n) – number of prime factors with multiplicity */
    seq_t bigom = bigomega(n);

    /* A056239(n) – fully additive with a(p) = π(p) (primepi(p)) */
    seq_t sumA = 0;
    uint64_t m = (uint64_t)n;

    while (m > 1) {
        uint64_t p  = smallest_prime_factor(m);
        uint64_t cnt = 0;

        while (m % p == 0) {
            m /= p;
            ++cnt;
        }

        seq_t pi_p = compute_pi(p);
        sumA += (seq_t)cnt * pi_p;
    }

    return gcd(bigom, sumA);
}

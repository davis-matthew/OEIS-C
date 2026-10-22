/*
 * A331415  Sum of prime factors minus sum of prime indices of n.
 * https://oeis.org/A331415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{p^e‖n} e·(p − π(p)) where π(p) is the prime index of p
 * Range:     u8 n <= 330, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/compute_pi.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;         /* a(1) = 0 */

    seq_t sumPrime = 0;
    seq_t sumIndex = 0;
    idx_t m = n;

    /* factor 2 (the only even prime) */
    if ((m & 1) == 0) {
        idx_t exp = 0;
        while ((m & 1) == 0) {
            m >>= 1;
            exp++;
        }
        seq_t prime = (seq_t)2;
        seq_t idx = compute_pi(2);
        sumPrime += prime * exp;
        sumIndex += idx * exp;
    }

    /* odd prime factors */
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                exp++;
            }
            seq_t prime = (seq_t)p;
            seq_t idx = compute_pi(p);
            sumPrime += prime * exp;
            sumIndex += idx * exp;
        }
    }

    /* remaining prime factor > 1 */
    if (m > 1) {
        seq_t prime = (seq_t)m;
        seq_t idx = compute_pi(m);
        sumPrime += prime;
        sumIndex += idx;
    }

    return sumPrime - sumIndex;
}

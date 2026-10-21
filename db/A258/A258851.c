/*
 * A258851  The pi-based arithmetic derivative of n: a(p) = pi(p) for p prime,
 *          a(u*v) = a(u)*v + u*a(v), where pi = A000720.
 * https://oeis.org/A258851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = n * Σ_{p^e||n} e * pi(p)/p (pi = A000720) Implements the
 *            prime‑factorial definition using a(u*v) = a(u)*v + u*a(v). a(0)
 *            = 0, a(1) = 0
 * Range:     u8 n <= 95, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _smallest_prime_factor, compute_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_smallest_prime_factor.h"
#include "../utils/compute_pi.h"

seq_t a(idx_t n) {
    if (n <= 1) return 0;         /* a(0)=0, a(1)=0 */

    seq_t result = 0;
    idx_t m = n;

    while (m > 1) {
        /* Obtain a prime divisor of m         */
        uint64_t p = _smallest_prime_factor(m);

        /* Count its exponent               */
        idx_t exp = 0;
        while (m % p == 0) {
            m /= p;
            exp++;
        }

        /* Contribution of this factor:     */
        seq_t pi_val   = compute_pi((idx_t)p);  /* pi(p) */
        seq_t term     = (seq_t)exp * pi_val * (seq_t)(n / p);
        result += term;
    }

    return result;
}

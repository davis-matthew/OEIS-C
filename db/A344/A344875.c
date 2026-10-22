/*
 * A344875  Multiplicative with a(2^e) = 2^(1+e) - 1, and a(p^e) = p^e - 1 for
 *          odd primes p.
 * https://oeis.org/A344875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p^e||n} f(p^e) where f(2^e) = 2^(e+1) - 1, f(p^e) =
 *            p^e - 1 for odd primes p
 * Range:     u8 n <= 151, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the multiplicative definition by simple trial division.  */
#include <stdint.h>

seq_t a(idx_t n) {
    seq_t ans = 1;
    uint64_t tmp = (uint64_t)n;
    while (tmp > 1) {
        uint64_t p = 0, e = 0;

        /* find smallest prime factor of tmp */
        if (tmp % 2 == 0) {
            p = 2;
        } else {
            for (uint64_t d = 3; d * d <= tmp; d += 2) {
                if (tmp % d == 0) { p = d; break; }
            }
            if (p == 0) p = tmp;          /* tmp itself is prime */
        }

        /* count exponent e of p */
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }

        /* compute f(p^e) */
        seq_t term = 1;
        if (p == 2) {
            seq_t pow2 = 1;
            for (unsigned i = 0; i < e + 1; ++i) pow2 *= 2;
            term = pow2 - 1;
        } else {
            seq_t pw = 1;
            for (unsigned i = 0; i < e; ++i) pw *= p;
            term = pw - 1;
        }

        ans *= term;
    }
    return ans;
}

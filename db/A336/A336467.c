/*
 * A336467  Fully multiplicative with a(2) = 1 and a(p) = A000265(p+1) for odd
 *          primes p, with A000265(k) giving the odd part of k.
 * https://oeis.org/A336467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p odd prime | n} A000265(p+1)^{v_p(n)}, with a(2)=1.
 *            (OEIS A336467)
 * Range:     u8 n <= 520, u16 n <= 8639, u32 n <= 8639, u64 n <= 8639,
 *            u128 n <= 8639, big any n
 * Helpers:   _smallest_prime_factor, odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_smallest_prime_factor.h"
#include "../utils/odd_part.h"

seq_t a(idx_t n) {
    seq_t result = 1;
    uint64_t m = (uint64_t)n;
    if (m <= 1) return result;

    while (m > 1) {
        uint64_t spf = _smallest_prime_factor(m);

        if (spf == 2) {
            /* All powers of 2 contribute 1, so just strip them out. */
            while ((m & 1) == 0) m >>= 1;
            continue;
        }

        int exp = 0;
        while (m % spf == 0) {
            m /= spf;
            exp++;
        }

        /* odd_part(p+1) is the odd part of p+1. */
        seq_t odd = odd_part((seq_t)spf + 1);

        for (int i = 0; i < exp; ++i) result *= odd;
    }

    return result;
}

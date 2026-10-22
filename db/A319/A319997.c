/*
 * A319997  a(n) = Sum_{d|n, d is odd} mu(n/d)*d, where mu(n) is Moebius
 *          function A008683.
 * https://oeis.org/A319997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, changed
 * Formula:   a(n) = Sum_{d|n, d odd} mu(n/d)*d. Alternate closed form: a(n) =
 *            φ(n) for odd n, a(n) = φ(n) - 2·φ(n/2) for even n where φ is
 *            Euler’s totient function
 * Range:     i8 n <= 129, i16 n <= 9394, i32 n <= 9394, i64 n <= 9394,
 *            i128 n <= 9394, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* provides factor_u64() */

static seq_t phi(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* allocate enough space for prime factors (log₂n ≤ 64 for 64‑bit n) */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64(n, primes, exps, &cnt);

    seq_t result = (seq_t)n;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        result = result / p * (p - 1);
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* offset is 1; input <1 is unused */

    seq_t phi_n = phi(n);
    if ((n & 1ULL) == 1ULL) {       /* odd n */
        return phi_n;
    } else {                       /* even n */
        seq_t phi_half = phi(n >> 1);
        return phi_n - (seq_t)2 * phi_half;
    }
}

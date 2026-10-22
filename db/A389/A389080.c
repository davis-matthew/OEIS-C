/*
 * A389080  a(n) = gcd(sigma(n), A048250(n)), where sigma is the sum of
 *          divisors and A048250 is the sum of squarefree divisors.
 * https://oeis.org/A389080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(sigma(n), A048250(n)) where sigma(n) is the sum of
 *            divisors and A048250(n) is the sum of squarefree divisors
 * Range:     u8 n <= 137, u16 n <= 9504, u32 n <= 9504, u64 n <= 9504,
 *            u128 n <= 9504, big any n
 * Helpers:   factor_u64, gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/sigma_of.h"
#include "../utils/factor_u64.h"

/* a(n) = gcd( σ(n), ∏_{p|n}(1+p) ) */
seq_t a(idx_t n)
{
    /* Sum of divisors */
    seq_t sigma = sigma_of(n);

    /* Sum of squarefree divisors = product of (1 + p) over distinct primes p|n */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;

    if (n > 1) {
        factor_u64((uint64_t)n, pr, ex, &cnt);
    }

    seq_t sf = 1;
    for (size_t i = 0; i < cnt; ++i) {
        sf = sf * ((seq_t)pr[i] + 1);
    }

    return gcd(sigma, sf);
}

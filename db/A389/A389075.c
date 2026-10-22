/*
 * A389075  Denominator of ratio A034448(n) / A048250(n), where A034448 is the
 *          sum of unitary divisors and A048250 is the sum of squarefree
 *          divisors.
 * https://oeis.org/A389075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of A034448(n)/A048250(n)
 * Range:     u8 n <= 549, u16 n <= 8293, u32 n <= 8293, u64 n <= 8293,
 *            u128 n <= 8293, big any n
 * Helpers:   factor_u64, pow_uint, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A034448(n) = ∏_{p^e‖n} (1 + p^e)  and  A048250(n) = ∏_{p|n} (1 + p) */
/* a(n) = (∏_{p|n} (1 + p)) / gcd(∏_{p|n} (1 + p), ∏_{p^e‖n} (1 + p^e)) */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"
#include "../utils/seq_gcd.h"

seq_t a(idx_t n) {
    /* Handle n = 0 (out of range) just in case */
    if (n == 0) return 0;

    /* Factorization: primes and exponents arrays */
    /* For numbers up to 2^64, the number of distinct primes is < 64 */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t N = (seq_t)1;  /* numerator product = ∏ (1 + p^e) */
    seq_t D = (seq_t)1;  /* denominator product = ∏ (1 + p) */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        seq_t termN = pow_uint(p, e) + (seq_t)1;   /* 1 + p^e */
        seq_t termD = (seq_t)p + (seq_t)1;         /* 1 + p */

        N = N * termN;
        D = D * termD;
    }

    /* Reduce the fraction: cancel common factors */
    seq_t g = seq_gcd(N, D);
    seq_t result = D / g;   /* D/g is already reduced */

    return result;
}

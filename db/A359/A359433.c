/*
 * A359433  Dirichlet inverse of A071773.
 * https://oeis.org/A359433
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = ∏_{p^e∥n} (-1)^e * (1-p)^{⌊e/2⌋} (Dirichlet inverse of
 *            A071773)
 * Range:     i8 n <= 8831, i16 n <= 8831, i32 n <= 8831, i64 n <= 8831,
 *            i128 n <= 8831, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* off–offset guard */
    if (n == 1) return (seq_t)1;  /* a(1) = 1 */

    uint64_t num = (uint64_t)n;

    /* buffers for factorization – any n <= UINT64_MAX has at most 64 distinct primes */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;

    factor_u64(num, primes, exps, &pcnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];

        /* (1 - p) in seq_t */
        seq_t base = (seq_t)1 - (seq_t)p;

        /* exponent floor(e/2) */
        uint64_t k = e >> 1;

        /* compute base^k */
        seq_t pow_base = (seq_t)1;
        for (uint64_t j = 0; j < k; ++j)
            pow_base *= base;

        /* multiply by (-1)^e */
        if (e & 1U)
            pow_base = -pow_base;

        result *= pow_base;
    }

    return result;
}

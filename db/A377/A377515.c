/*
 * A377515  The largest divisor of n that is a term in A276078.
 * https://oeis.org/A377515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product_{p^e||n} p^min(pi(p), e) (pi(p) is the prime
 *            counting function, the index of prime p)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, prime_pi, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/prime_pi.h"
#include "../utils/seq_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)1;          /* a(1)=1 */

    /* Allocate space for prime factors – 64 is enough for 64‑bit n */
    const size_t MAX_FACTORS = 64;
    uint64_t *pr = (uint64_t *)malloc(sizeof(uint64_t) * MAX_FACTORS);
    uint64_t *ex = (uint64_t *)malloc(sizeof(uint64_t) * MAX_FACTORS);
    size_t cnt = 0;

    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];

        seq_t pi_p = prime_pi((idx_t)p);      /* pi(p) */
        seq_t exp = (seq_t)e;
        if (exp > pi_p) exp = pi_p;

        seq_t pow = seq_pow((seq_t)p, (idx_t)exp);
        result = seq_mul(result, pow);
    }

    free(pr);
    free(ex);
    return result;
}

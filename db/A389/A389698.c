/*
 * A389698  Multiplicative sequence a(n) with a(p^e) = 4 * e^2 + 2 for prime p
 *          and e > 0.
 * https://oeis.org/A389698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   multiplicative sequence a(n) * with a(1) = 1 and a(p^e) = 4*e^2
 *            + 2 for every prime p and * exponent e > 0. The function returns
 *            a(n) for n ≥ 1
 * Range:     u8 n <= 35, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9977 (2026-11-01)
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "../utils/build_primes.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* Undefined for 0, but safe to return 0 */

    static idx_t *primes   = NULL;
    static size_t primes_cnt = 0;
    static idx_t  prime_limit = 0;

    /* Rebuild the prime list if we ask for a larger n */
    if (n > prime_limit || primes == NULL)
    {
        if (primes != NULL) { free(primes); primes = NULL; }
        build_primes(n, &primes, &primes_cnt);   /* primes array ends with NULL */
        prime_limit = n;
    }

    uint64_t nn = (uint64_t)n;
    seq_t result = 1;

    /* Factorize n using the cached prime list */
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= nn; ++i)
    {
        uint64_t p = (uint64_t)primes[i];
        if (nn % p == 0)
        {
            uint64_t e = 0;
            while (nn % p == 0)
            {
                nn /= p;
                ++e;
            }
            seq_t val = (seq_t)4 * (seq_t)e * (seq_t)e + 2;
            result = seq_mul(result, val);
        }
    }

    /* If a prime > sqrt(n) remains, it occurs once */
    if (nn > 1)
    {
        seq_t val = (seq_t)4 * (seq_t)1 * (seq_t)1 + 2;  /* 4*1^2+2 = 6 */
        result = seq_mul(result, val);
    }

    return result;
}

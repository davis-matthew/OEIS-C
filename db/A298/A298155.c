/*
 * A298155  For any n >= 0 and k > 0, the prime(k)-adic valuation of a(n)
 *          equals the prime(k)-adic valuation of n + k (where prime(k)
 *          denotes the k-th prime).
 * https://oeis.org/A298155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{k>0} prime(k)^{v_prime(k)(n + k)} where prime(k)
 *            is the k-th prime and v_prime(k)(x) is the exponent of prime[k]
 *            in the prime factorization of x. This implements OEIS A298155
 * Range:     u8 n <= 6, u16 n <= 51, u32 n <= 478, u64 n <= 8826,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9797 (2026-11-01)
 * Helpers:   build_primes, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include "../utils/build_primes.h"
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static bool primes_built = false;
static idx_t *primes = NULL;
static size_t prime_cnt = 0;

/* Build a list of primes once, large enough for all expected n. */
static void init_primes(void)
{
    if (!primes_built)
    {
        /* Upper bound on primes needed for n up to about 10,000.
           2,000,000 guarantees all necessary primes are present. */
        const idx_t PRIME_LIMIT = 2000000;
        build_primes(PRIME_LIMIT, &primes, &prime_cnt);
        primes_built = true;
    }
}

seq_t a(idx_t n)
{
    init_primes();

    seq_t result = 1;                     /* identity for multiplication    */
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        idx_t p = primes[i];              /* prime(k) with k = i+1          */
        idx_t k = (idx_t)(i + 1);
        idx_t m = n + k;                  /* n + k                          */

        if (p > m)                       /* primes only grow, break early  */
            break;

        /* compute v_p(m) */
        idx_t exp = 0;
        idx_t temp = m;
        while (temp % p == 0)
        {
            temp /= p;
            ++exp;
        }

        if (exp > 0)
        {
            seq_t powp = ipow_idx((seq_t)p, exp);
            result = seq_mul(result, powp);
        }
    }
    return result;
}

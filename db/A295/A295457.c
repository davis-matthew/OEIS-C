/*
 * A295457  a(n) = (15*n)!*(4*n)!/((8*n)!*(6*n)!*(5*n)!).
 * https://oeis.org/A295457
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (15*n)! * (4*n)! / ((8*n)! * (6*n)! * (5*n)!)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 8,
 *            big any n
 * Helpers:   build_primes_upto, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses prime‑factorisation of the factorials to keep */
/* intermediate values small; works for any idx_t/seq_t size. */

#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"
#include "../utils/build_primes_upto.h"
#include <stddef.h>
#include <stdlib.h>

static uint64_t *prime_cache      = NULL; /* primes list */
static size_t   prime_cache_len   = 0;    /* count of primes */
static idx_t    prime_cache_limit = 0;    /* largest prime considered */

/* Ensure we have all primes up to at least 'limit' */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_cache_limit)
        return;

    if (prime_cache)
        free(prime_cache);

    build_primes_upto(limit, &prime_cache, &prime_cache_len);
    prime_cache_limit = limit;
}

seq_t a(idx_t n)
{
    /* Base case: a(0)=1 */
    if (n == 0)
        return (seq_t)1;

    /* 1. Find needed primes: up to 15*n */
    idx_t max_val = 15 * n;
    ensure_primes(max_val);

    seq_t result = (seq_t)1;

    /* 2. For each prime p ≤ max_val compute its exponent in the formula */
    for (size_t i = 0; i < prime_cache_len; ++i) {
        idx_t p = prime_cache[i];
        if (p > max_val)
            break;

        idx_t exp = 0;
        for (idx_t pk = p; pk <= max_val; /* empty */) {
            exp += (15 * n) / pk - (8 * n) / pk
                 -  (6 * n) / pk - (5 * n) / pk
                 +  (4 * n) / pk;

            /* Prevent overflow in next multiplication */
            if (pk > max_val / p)
                break;
            pk *= p;
        }

        if (exp > 0) {
            seq_t pw = seq_pow((seq_t)p, exp);
            result = seq_mul(result, pw);
        }
    }

    return result;
}

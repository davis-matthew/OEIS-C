/*
 * A295452  a(n) = (10*n)!*(6*n)!*n!/((7*n)!*(5*n)!*(3*n)!*(2*n)!).
 * https://oeis.org/A295452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A295452: a(n) = (10*n)!*(6*n)!*n!/((7*n)!*(5*n)!*(3*n)!*(2*n)!) */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_pow.h"

/* Exponent of prime p in m! */
static idx_t exp_factorial(idx_t m, idx_t p)
{
    idx_t exp = 0;
    idx_t power = p;
    while (power <= m) {
        exp += m / power;
        if (power > m / p) break;          /* avoid overflow of power *= p   */
        power *= p;
    }
    return exp;
}

/* Prime sieve cache. */
static uint64_t *prime_sieve = NULL;    /* array of primes */
static size_t prime_sieve_cnt = 0;     /* number of primes stored */
static idx_t  prime_sieve_max = 0;     /* maximum value sieved up to */

/* Ensure we have all primes <= limit in the cache. */
static void ensure_primes_up_to(idx_t limit)
{
    if (limit <= prime_sieve_max) return;

    size_t sz = (size_t)(limit + 1);
    bool *composite = (bool *)calloc(sz, sizeof(bool));
    if (!composite) return;   /* allocation failure – give up */

    idx_t i;
    for (i = 2; i * i <= limit; ++i) {
        if (!composite[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                composite[j] = true;
        }
    }

    size_t cnt = 0;
    for (i = 2; i <= limit; ++i)
        if (!composite[i]) ++cnt;

    uint64_t *pr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!pr) {
        free(composite);
        return;
    }

    size_t idx = 0;
    for (i = 2; i <= limit; ++i)
        if (!composite[i])
            pr[idx++] = (uint64_t)i;

    free(composite);
    free(prime_sieve);
    prime_sieve   = pr;
    prime_sieve_cnt = cnt;
    prime_sieve_max  = limit;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Make sure we have all primes needed. */
    ensure_primes_up_to(10 * n);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < prime_sieve_cnt; ++i) {
        idx_t p = prime_sieve[i];
        if (p > 10 * n) break;

        idx_t e =  exp_factorial(10 * n, p) + exp_factorial(6 * n, p)
                + exp_factorial(    n, p)
                - exp_factorial(7 * n, p) - exp_factorial(5 * n, p)
                - exp_factorial(3 * n, p) - exp_factorial(2 * n, p);

        if (e > 0) {
            seq_t pw = seq_pow((seq_t)p, e);
            result *= pw;
        }
    }

    return result;
}

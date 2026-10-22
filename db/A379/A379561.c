/*
 * A379561  a(n) = A003418(n+1)*H(n), where H(n) = 1 + 1/2 + ... + 1/n is the
 *          n-th harmonic number.
 * https://oeis.org/A379561
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003418(n+1) * H(n) = lcm(1…n+1) * (1 + 1/2 + … + 1/n) *
 *            where H(0) = 0 and n ≥ 1. The implementation uses a dynamic
 *            sieve * to generate all primes up to the maximum n+1 requested.
 *            The lcm is * computed as the product of prime powers
 *            p^⌊log_p(n+1)⌋, and the sum * Σ_{k=1}^{n} lcm(1…n+1)/k g...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   log_p_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/log_p_floor.h"

/* Static storage for prime list.  It grows only when a larger limit is
   queried. */
static uint64_t *prime_list = NULL;   /* array of primes */
static size_t   prime_cnt  = 0;       /* number of primes in prime_list */
static idx_t     sieve_limit = 0;     /* highest value for which primes are computed */

/* Allocate or enlarge an array of size_t elements.  Used for the
   temporary boolean array in the sieve. */
static void *xmalloc(size_t size)
{
    void *p = malloc(size);
    if (!p) abort();
    return p;
}

/* Ensure that all primes up to ‘limit’ are present in prime_list. */
static void
ensure_primes(idx_t limit)
{
    if (limit <= sieve_limit)
        return;

    /* Simple Sieve of Eratosthenes. */
    size_t max = (size_t)limit + 1;
    bool *is_prime = (bool *)xmalloc(max);
    for (size_t i = 0; i < max; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (size_t i = 2; i * i <= (size_t)limit; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= (size_t)limit; j += i)
                is_prime[j] = false;

    /* Count primes to allocate prime_list. */
    size_t new_cnt = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (is_prime[i]) ++new_cnt;

    uint64_t *new_list = NULL;
    if (new_cnt > 0)
        new_list = (uint64_t *)xmalloc(new_cnt * sizeof(uint64_t));

    size_t idx = 0;
    for (size_t i = 2; i <= (size_t)limit; ++i)
        if (is_prime[i])
            new_list[idx++] = (uint64_t)i;

    /* Replace old list. */
    free(prime_list);
    prime_list = new_list;
    prime_cnt  = new_cnt;
    sieve_limit = limit;

    free(is_prime);
}

/* Integer exponentiation for seq_t.  Base fits in uint64_t, exponent
   is non‑negative. */
static seq_t
pow_seq_uint(uint64_t base, int exp)
{
    seq_t result = 1;
    seq_t power  = base;
    while (exp > 0)
    {
        if (exp & 1)
            result *= power;
        power *= power;
        exp >>= 1;
    }
    return result;
}

/* Main sequence function.  Computes a(n) for n ≥ 1. */
seq_t
a(idx_t n)
{
    /* The OEIS definition uses offset 1: a(1) is the first term.  If
       n is 0 (outside the catalogue) return 0. */
    if (n == 0)
        return 0;

    /* Ensure primes up to n+1 are available. */
    idx_t limit = n + 1;
    ensure_primes(limit);

    /* Compute L = lcm(1 … n+1). */
    seq_t L = 1;
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t p = prime_list[i];
        int e = log_p_floor(p, limit);
        if (e > 0)
            L *= pow_seq_uint(p, e);
    }

    /* Sum_{k=1}^{n} L/k gives A003418(n+1) * H(n). */
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
        sum += L / k;

    return sum;
}

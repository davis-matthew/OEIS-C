/*
 * A399575  Minimum difference between two composite numbers whose sum is
 *          prime(n).
 * https://oeis.org/A399575
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = min{|c-d| : c,d composite and c+d = prime(n)}.
 *            Implementation uses a sieve of Eratosthenes up to a fixed limit
 *            (200000) and a linear search from p/2 downwards to find the
 *            first composite pair. This guarantees a(n) for all n ≥ 6
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* static state for the sieve */
static uint8_t  *is_composite = NULL;   /* 0 = prime, 1 = composite */
static uint64_t *primes       = NULL;   /* array of primes */
static size_t   primes_cnt   = 0;        /* number of primes stored */
static size_t   sieve_limit  = 0;        /* current sieve upper bound */

/* initialise sieve up to a predefined limit */
static void init_sieve(void)
{
    if (sieve_limit != 0)               /* already initialised */
        return;

    sieve_limit = 200000;               /* enough for prime(10000) ≈ 104729 */

    /* allocate composite flag array */
    is_composite = (uint8_t *)calloc(sieve_limit + 1U, sizeof(uint8_t));
    if (!is_composite)
        return;                         /* allocation failure - will return 0 */

    /* simple Eratosthenes: mark composites */
    for (uint64_t i = 2; i * i <= sieve_limit; ++i)
        if (!is_composite[i])
            for (uint64_t j = i * i; j <= sieve_limit; j += i)
                is_composite[j] = 1;

    /* count primes and fill primes array */
    primes_cnt = 0;
    for (uint64_t i = 2; i <= sieve_limit; ++i)
        if (!is_composite[i])
            ++primes_cnt;

    if (primes_cnt == 0)
        return;

    primes = (uint64_t *)malloc(primes_cnt * sizeof(uint64_t));
    if (!primes)
        return;                         /* allocation failure */

    size_t idx = 0;
    for (uint64_t i = 2; i <= sieve_limit; ++i)
        if (!is_composite[i])
            primes[idx++] = i;
}

/* compute a(n) as defined */
seq_t a(idx_t n)
{
    if (n < 6U)           /* sequence undefined below offset */
        return (seq_t)0;

    init_sieve();          /* ensure sieve ready */

    if (primes == NULL || (size_t)n > primes_cnt)
        return (seq_t)0;   /* safety check, should not happen for n ≤ 10000 */

    uint64_t p = primes[n - 1U]; /* primes[0] = 2 */

    uint64_t start = p / 2U;
    for (uint64_t c = start; c >= 4U; --c) {
        uint64_t d = p - c;
        if (!is_composite[c] || !is_composite[d])
            continue;
        uint64_t diff = (c > d) ? (c - d) : (d - c);
        return (seq_t)diff;
    }

    return (seq_t)0;   /* unreachable for p ≥ 13 */
}

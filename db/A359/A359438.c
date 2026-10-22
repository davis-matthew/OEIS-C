/*
 * A359438  For n >= 0, let S be the sequence of numbers m such that (m^2 -
 *          2*n^2 + 1)/2 is a square. Then a(n) is the number k such that S(j)
 *          = 6*S(j-k) - S(j-2k) for all j for which S(j-2k) is defined.
 * https://oeis.org/A359438
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000005(2*n^2 - 1)
 * Range:     u8 n <= 5, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  i.e. number of divisors of (2*n^2 - 1)          */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* ---- Prime sieve helpers ----------------------------------------- */
static uint64_t *prime_list   = NULL;  /* array of primes */
static size_t   prime_count   = 0;    /* number of stored primes   */
static idx_t    prime_limit   = 0;    /* current maximum prime limit */

/*  Build or extend the prime table up to 'limit'.  */
static void
ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;                                   /* already sufficient */

    if (limit < 2) {
        prime_limit = limit;
        free(prime_list);
        prime_list = NULL;
        prime_count = 0;
        return;
    }

    size_t sz = (size_t)(limit + 1);
    unsigned char *is_prime = (unsigned char *)malloc(sz);
    if (!is_prime) exit(EXIT_FAILURE);

    for (size_t i = 0; i < sz; ++i)
        is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (uint64_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (uint64_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;

    /* Count primes */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++cnt;

    uint64_t *plist = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!plist) {
        free(is_prime);
        exit(EXIT_FAILURE);
    }

    size_t pos = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            plist[pos++] = i;

    free(is_prime);
    free(prime_list);

    prime_list = plist;
    prime_count = cnt;
    prime_limit = limit;
}

/* ------------------------------------------------------------------- */

seq_t
a(idx_t n)
{
    /* Special case a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* Compute m = 2*n^2 - 1 */
    seq_t m = (seq_t)2 * (seq_t)n * (seq_t)n - (seq_t)1;

    /* Pre‑compute primes up to ⌊√m⌋ */
    idx_t max_prime_needed = (idx_t)floor_sqrt(m);
    ensure_primes(max_prime_needed);

    seq_t remaining   = m;
    seq_t dcount      = (seq_t)1;   /* divisor count */

    /* Trial division by all stored primes ≤ √remaining   */
    for (size_t i = 0; i < prime_count &&
                       (seq_t)prime_list[i] * (seq_t)prime_list[i] <= remaining;
         ++i) {
        uint64_t p = prime_list[i];
        seq_t exp = (seq_t)0;
        while (remaining % (seq_t)p == (seq_t)0) {
            remaining /= (seq_t)p;
            ++exp;
        }
        if (exp > (seq_t)0)
            dcount *= (exp + (seq_t)1);
    }

    /* If remaining > 1, it is a prime factor of exponent 1 */
    if (remaining > (seq_t)1)
        dcount *= (seq_t)2;

    return dcount;
}

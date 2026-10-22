/*
 * A331020  Values k for successive maximal records of the function A defined
 *          as A(prime(k)) = log(prime(k)) - prime(k)/Pi(prime(k)) where
 *          Pi(prime(k)) is number of primes <= prime(k).
 * https://oeis.org/A331020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) is the index k of the prime at which the value A(prime(k))
 *            = log(prime(k)) - prime(k)/π(prime(k)) attains a new record
 *            among all primes examined so far
 * Range:     u8 n <= 31, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A331020: successive k for maximal records of A.  The sequence
    is finite; the last record is reached at prime 24137 (k = 2688). */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "stdbool.h"

static bool seq_ready = false;
static seq_t *seq = NULL;
static size_t seq_len = 0;

/* Simple sieve of Eratosthenes returning a dynamic array of primes
   up to 'limit'.  The caller is responsible for freeing primes. */
static uint64_t *sieve_primes(uint64_t limit, size_t *pn)
{
    uint8_t *is_prime = (uint8_t *)malloc(limit + 1);
    if (!is_prime) return NULL;
    for (uint64_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (uint64_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (uint64_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;

    /* Count primes first */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i) if (is_prime[i]) ++cnt;

    uint64_t *primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!primes) { free(is_prime); return NULL; }

    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i)
        if (is_prime[i]) primes[idx++] = i;

    free(is_prime);
    *pn = cnt;
    return primes;
}

static void compute_sequence(void)
{
    if (seq_ready) return;

    /* Plan: sieve up to a bound that contains the last prime that gives
       a record (24137).  300000 is safely large. */
    const uint64_t bound = 300000;
    size_t prime_count;
    uint64_t *primes = sieve_primes(bound, &prime_count);
    if (!primes) return;  /* allocation failure; sequence stays empty */

    /* Now iterate over primes, compute A(p) and record new maxima. */
    double current_max = -1e100;  /* very small */
    size_t records_est = prime_count / 10;  /* rough estimate */
    seq_t *tmp = (seq_t *)malloc(records_est * sizeof(seq_t));
    size_t records = 0;

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = primes[i];
        size_t k = i + 1;  /* index of the prime (1‑based) */
        double ap = log((double)p) - ((double)p) / (double)k;
        if (ap > current_max) {
            current_max = ap;
            if (records == records_est) {
                records_est *= 2;
                tmp = (seq_t *)realloc(tmp, records_est * sizeof(seq_t));
            }
            tmp[records++] = (seq_t)k;
        }
    }

    free(primes);
    seq = tmp;
    seq_len = records;
    seq_ready = true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* out of range, but return 0 */
    compute_sequence();
    if ((size_t)n > seq_len) return 0;
    return seq[(size_t)n - 1];
}

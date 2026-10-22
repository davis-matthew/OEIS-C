/*
 * A325314  a(n) = n - A162296(n), where A162296(n) is the sum of divisors of
 *          n that have a square factor.
 * https://oeis.org/A325314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n - A162296(n) * A162296(n) = sum of divisors of n that
 *            have a square factor * = sigma(n) - prod_{p|n}(1+p) * Hence a(n)
 *            = n - sigma(n) + prod_{p|n}(1+p) * The implementation uses a
 *            linear sieve for smallest prime factors * and multiplicative
 *            formulas for sigma(n) and prod(1+p)
 * Range:     i8 n <= 95, i16 n <= 8238, i32 n <= 8238, i64 n <= 8238,
 *            i128 n <= 8238, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static uint64_t *spf = NULL;   /* smallest prime factor */
static uint64_t *primes = NULL;/* primes up to current limit */
static size_t primes_cnt = 0;
static idx_t spf_limit = 0;

/* Ensure that spf[] and primes[] are computed up to at least 'limit' */
static void ensure_spf(idx_t limit)
{
    if (limit <= spf_limit)
        return;

    uint64_t *new_spf = (uint64_t*)calloc(limit + 1, sizeof(uint64_t));
    if (!new_spf) exit(EXIT_FAILURE);

    uint64_t *new_primes = NULL;
    size_t new_cnt = 0;
    /* Linear sieve */
    for (idx_t i = 2; i <= limit; ++i) {
        if (!new_spf[i]) {
            new_spf[i] = i;
            new_primes = (uint64_t*)realloc(new_primes, (new_cnt + 1) * sizeof(uint64_t));
            new_primes[new_cnt++] = i;
        }
        for (size_t j = 0; j < new_cnt && new_primes[j] <= new_spf[i] && i * new_primes[j] <= limit; ++j) {
            new_spf[i * new_primes[j]] = new_primes[j];
        }
    }

    /* Replace old arrays */
    free(spf);
    free(primes);
    spf = new_spf;
    primes = new_primes;
    primes_cnt = new_cnt;
    spf_limit = limit;
}

/* Compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* not used for offset 1, but safe */

    ensure_spf(n);

    uint64_t x = (uint64_t)n;
    seq_t sigma = 1;        /* product over primes of sum of powers */
    seq_t rad_prod = 1;     /* product over (1+p) */

    while (x > 1) {
        uint64_t p = spf[x];
        uint64_t pow_p = 1;
        seq_t sum_p = 1;
        while (x % p == 0) {
            x /= p;
            pow_p *= p;
            sum_p += pow_p;
        }
        sigma *= sum_p;
        rad_prod *= (seq_t)(p + 1);
    }

    seq_t result = (seq_t)n - sigma + rad_prod;
    return result;
}

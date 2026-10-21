/*
 * A269703  Numbers k such that prime(k) == 1 (mod 7).
 * https://oeis.org/A269703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that prime(k) ≡ 1 (mod 7) */
/* Implementation: iteratively generate primes and count indices
   whose primes are congruent to 1 mod 7. */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* helper: generate all primes up to 'limit' using a simple sieve.
   Returns a heap-allocated array of primes (uint32_t) and sets *cnt_out
   to the number of primes generated. The caller must free the returned
   array when no longer needed. */
static uint32_t *sieve_primes(idx_t limit, size_t *cnt_out)
{
    /* mark composite numbers */
    uint8_t *mark = (uint8_t *)malloc((size_t)(limit + 1));
    if (!mark) abort();
    memset(mark, 0, (size_t)(limit + 1));

    /* estimate number of primes (pi(limit) < limit/ln(limit) < limit/10 for limit ≤ 2M) */
    size_t est = (size_t)(limit / 10) + 100;
    uint32_t *primes = (uint32_t *)malloc(est * sizeof(uint32_t));
    if (!primes) abort();

    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!mark[i]) {
            if (cnt == est) {
                est *= 2;
                primes = (uint32_t *)realloc(primes, est * sizeof(uint32_t));
                if (!primes) abort();
            }
            primes[cnt++] = (uint32_t)i;
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    mark[j] = 1;
            }
        }
    }

    free(mark);
    *cnt_out = cnt;
    return primes;
}

/* main sequence function -------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n = 0, return 0 */

    /* static cache of produced primes */
    static idx_t cache_limit = 0;
    static uint32_t *primes = NULL;
    static size_t prime_cnt = 0;

    /* estimate the k-th prime index needed: about 6*n */
    idx_t needed_prime_index = 6 * n;

    /* initial prime bound estimate: 120*n suffices for n ≤ 10000 */
    idx_t prime_limit = (n < 10) ? 200 : n * 120;

    /* rebuild cache if needed */
    if (prime_cnt < (size_t)needed_prime_index || prime_limit > cache_limit) {
        free(primes);
        primes = sieve_primes(prime_limit, &prime_cnt);
        cache_limit = prime_limit;
    }

    seq_t found = 0;
    for (idx_t k = 1; k <= prime_cnt; ++k) {
        if ((primes[k - 1] % 7ULL) == 1ULL) {
            ++found;
            if (found == n) return (seq_t)k;
        }
    }

    /* if we haven't found n, increase the bound and retry */
    while (1) {
        prime_limit *= 2;
        free(primes);
        primes = sieve_primes(prime_limit, &prime_cnt);
        cache_limit = prime_limit;

        found = 0;
        for (idx_t k = 1; k <= prime_cnt; ++k) {
            if ((primes[k - 1] % 7ULL) == 1ULL) {
                ++found;
                if (found == n) return (seq_t)k;
            }
        }
    }
}

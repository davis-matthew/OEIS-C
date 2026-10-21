/*
 * A268467  Smallest prime that is the (sum, k*prime(k),k=m,..n+m-1) for some
 *          m, or 0 if no such m exists.
 * https://oeis.org/A268467
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 28, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A268467:
   Smallest prime that equals the sum of n consecutive terms of
   k*prime(k) for some starting index m (m ≥ 1). If no such m exists,
   return 0.  The OEIS offset is 1.  The implementation precomputes a
   prime list and the cumulative sums of k*prime(k) and then scans
   all possible starting positions for each requested n. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static bool initialized = false;
static uint32_t *primes = NULL;          /* primes[0] = 2, primes[1] = 3, ... */
static seq_t *prefix_sum = NULL;         /* prefix_sum[i] = Σ_{k=1}^{i} k*prime(k) */
static size_t prime_cnt = 0;

#define MAX_PRIMES 200000U           /* number of primes to precompute */

/* Allocate and initialize all required data structures */
static void init(void)
{
    if (initialized) return;

    /* 200,000th prime is 2,593,973, so sieve up to 25,000,000 is safe. */
    const uint32_t sieve_limit = 25000000U;
    uint8_t *sieve = (uint8_t *)malloc((size_t)sieve_limit + 1);
    if (!sieve) { exit(1); }
    memset(sieve, 0, (size_t)sieve_limit + 1);
    sieve[0] = sieve[1] = 1;
    for (uint32_t i = 2; (uint64_t)i * i <= sieve_limit; ++i) {
        if (!sieve[i]) {
            for (uint32_t j = i * i; j <= sieve_limit; j += i)
                sieve[j] = 1;
        }
    }

    primes = (uint32_t *)malloc((size_t)MAX_PRIMES * sizeof(uint32_t));
    if (!primes) { free(sieve); exit(1); }
    size_t cnt = 0;
    for (uint32_t i = 2; i <= sieve_limit && cnt < MAX_PRIMES; ++i) {
        if (!sieve[i]) primes[cnt++] = i;
    }
    if (cnt < MAX_PRIMES) { free(sieve); free(primes); exit(1); }
    prime_cnt = cnt;

    prefix_sum = (seq_t *)malloc((size_t)(prime_cnt + 1) * sizeof(seq_t));
    if (!prefix_sum) { free(sieve); free(primes); exit(1); }
    prefix_sum[0] = 0;
    for (size_t k = 1; k <= prime_cnt; ++k) {
        /* prefix_sum[k] = prefix_sum[k-1] + k * primes[k-1] */
        seq_t term = ((seq_t)k) * ((seq_t)primes[k-1]);
        prefix_sum[k] = prefix_sum[k-1] + term;
    }

    free(sieve);
    initialized = true;
}

seq_t a(idx_t n)
{
    init();

    /* Scan all possible starting positions m */
    for (idx_t m = 1; m + n - 1 <= (idx_t)prime_cnt; ++m) {
        seq_t sum = prefix_sum[m + n - 1] - prefix_sum[m - 1];
        if (is_prime_big(sum)) return sum;
    }
    return 0;
}

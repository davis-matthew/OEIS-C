/*
 * A253138  Number of ways to represent the n-th prime as the arithmetic mean
 *          of two semiprimes.
 * https://oeis.org/A253138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to express the n-th prime as the
 *            arithmetic mean of two semiprimes
 * Range:     u8 n <= 432, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) by generating all semiprimes up to 2·prime(n) and counting unordered pairs summing to 2·prime(n). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/get_prime.h"
#include "../utils/build_primes_upto.h"

static size_t precomp_limit = 0;
static uint8_t *semiprime_arr = NULL;
static uint64_t *primes = NULL;
static size_t primes_cnt = 0;

static void ensure_precomp(idx_t n)
{
    /* Determine maximum needed limit for given n and rebuild data if necessary. */
    uint64_t p = get_prime(n);
    size_t target = (size_t)(2 * p);
    if (target <= precomp_limit) return;

    /* Free old allocations if any. */
    if (semiprime_arr) free(semiprime_arr);
    if (primes) free((void *)primes);

    /* Rebuild primes up to the new target limit. */
    build_primes_upto((idx_t)target, (uint64_t **)&primes, &primes_cnt);
    precomp_limit = target;

    /* Allocate semiprime boolean array. */
    semiprime_arr = (uint8_t *)calloc(precomp_limit + 1, sizeof(uint8_t));
    if (!semiprime_arr) return;  /* allocation failure: handled implicitly by returning 0 count later */

    /* Mark all semiprimes (product of two primes, allowing repetition) up to target. */
    for (size_t i = 0; i < primes_cnt; ++i) {
        for (size_t j = i; j < primes_cnt; ++j) {
            uint64_t prod = primes[i] * primes[j];
            if (prod > precomp_limit) break;
            semiprime_arr[prod] = 1;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety, though offset starts at 1 */
    ensure_precomp(n);

    uint64_t p = get_prime(n);
    uint64_t sum = 2 * p;
    seq_t count = 0;

    for (uint64_t a_val = 4; a_val <= sum / 2; ++a_val) {
        if (semiprime_arr[a_val] && semiprime_arr[sum - a_val]) {
            count++;
        }
    }
    return count;
}

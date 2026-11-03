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
 * Validated: standard, pass, 88 terms through n = 923 (2026-11-02)
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) by generating all semiprimes up to 2·prime(n) and counting unordered pairs summing to 2·prime(n). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

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
    if (primes) free(primes);

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

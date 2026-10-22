/*
 * A355524  Minimal difference between adjacent prime indices of n > 1, or 0
 *          if n is prime.
 * https://oeis.org/A355524
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computation: For n ≥ 2, return 0 if n is prime.
 * Otherwise factor n, collect the indices of all prime factors
 * (the index of a prime p is the number of primes ≤ p),
 * sort these indices, and return the smallest difference between
 * adjacent indices.  If all indices are equal (i.e., n is a power of a single prime),
 * the function returns 0 as well.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Utility from the utils library */
#include "../utils/is_prime.h"

/* Static cache for prime counting (prefix sums) */
static uint32_t *prime_cnt = NULL;   /* prime_cnt[i] = π(i) */
static idx_t     prime_limit = 0;    /* size of prime_cnt */

static void ensure_prime_limit(idx_t limit)
{
    if (prime_limit >= limit) return;

    /* Allocate new array with space up to 'limit' */
    uint32_t *new_cnt = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    if (!new_cnt) return;            /* out of memory: will crash the program */

    /* Sieve of Eratosthenes up to 'limit'. */
    bool *composite = (bool *)calloc(limit + 1, sizeof(bool));
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (!composite[i]) {
            for (idx_t j = i * i; j <= limit; j += i) {
                composite[j] = true;
            }
        }
    }

    /* Prefix sums of primes. */
    new_cnt[0] = 0;
    new_cnt[1] = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        new_cnt[i] = new_cnt[i - 1] + (!composite[i] ? 1 : 0);
    }

    free(composite);
    free(prime_cnt);
    prime_cnt = new_cnt;
    prime_limit = limit;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;         /* undefined for n < 2 */

    /* Return 0 immediately for prime n. */
    if (is_prime(n)) return 0;

    /* Ensure we can access prime indices up to max prime factor. */
    ensure_prime_limit(n);

    /* Factor n and collect prime indices. */
    idx_t temp = n;
    seq_t idxs[64];
    size_t idx_cnt = 0;

    /* Factor 2 separately. */
    if ((temp & 1ULL) == 0) {
        seq_t idx = (seq_t)prime_cnt[2];   /* index of prime 2 is 1 */
        size_t exp = 0;
        while ((temp & 1ULL) == 0) {
            temp >>= 1;
            ++exp;
        }
        for (size_t k = 0; k < exp; ++k) idxs[idx_cnt++] = idx;
    }

    /* Factor odd primes up to sqrt(temp). */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            seq_t idx = (seq_t)prime_cnt[p];
            size_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            for (size_t k = 0; k < exp; ++k) idxs[idx_cnt++] = idx;
        }
    }

    /* Remaining prime factor > sqrt(temp), if any. */
    if (temp > 1) {
        seq_t idx = (seq_t)prime_cnt[temp];
        idxs[idx_cnt++] = idx;
    }

    /* If less than two distinct indices, return 0. */
    if (idx_cnt <= 1) return 0;

    /* Sort the indices (insertion sort). */
    for (size_t i = 1; i < idx_cnt; ++i) {
        seq_t key = idxs[i];
        size_t j = i;
        while (j > 0 && idxs[j - 1] > key) {
            idxs[j] = idxs[j - 1];
            --j;
        }
        idxs[j] = key;
    }

    /* Find minimal difference between successive indices. */
    seq_t min_diff = idxs[1] - idxs[0];
    if (min_diff == 0) return 0;

    for (size_t i = 1; i + 1 < idx_cnt; ++i) {
        seq_t diff = idxs[i + 1] - idxs[i];
        if (diff < min_diff) {
            min_diff = diff;
            if (min_diff == 0) return 0;
        }
    }

    return min_diff;
}

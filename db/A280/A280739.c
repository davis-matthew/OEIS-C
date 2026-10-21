/*
 * A280739  a(n) is the smallest composite number > a(n-1) for which the sum
 *          of the prime factors (counted with multiplicity) is n.
 * https://oeis.org/A280739
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest composite > a(n-1) whose prime factors (counted
 *            with multiplicity) sum to n (offset 4)
 * Range:     u8 n <= 21, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   sumPrimeFactors64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/sumPrimeFactors64.h"

/* Simple primality test for 64‑bit integers. */
static bool isPrime64(uint64_t n)
{
    if (n < 2) return false;
    if (n < 4) return true;          /* 2, 3 */
    if ((n & 1ULL) == 0ULL || (n % 3ULL) == 0ULL) return false;
    for (uint64_t i = 5ULL; i * i <= n; i += 6ULL) {
        if (n % i == 0ULL || n % (i + 2ULL) == 0ULL) return false;
    }
    return true;
}

/* Cache of previously computed a(k) values. */
static seq_t *a_cache = NULL;
static size_t cache_len = 0;          /* number of allocated slots */

/* Ensure the cache holds indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_len <= (size_t)n) {
        size_t newlen = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, newlen * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        a_cache = tmp;
        for (size_t i = cache_len; i < newlen; ++i) a_cache[i] = (seq_t)0;
        cache_len = newlen;
    }
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    if (n < 4) return (seq_t)0;        /* undefined below the offset */

    ensure_cache(n);

    if (a_cache[n] != (seq_t)0) return a_cache[n];

    /* Compute all missing terms up to n. */
    for (idx_t k = 4; k <= n; ++k) {
        if (a_cache[k] != (seq_t)0) continue;   /* already known */

        if (k == 4) {                         /* base value */
            a_cache[4] = (seq_t)4;
            continue;
        }

        uint64_t prev = (uint64_t)a_cache[k - 1];
        uint64_t cand = prev + 1ULL;

        while (true) {
            if (!isPrime64(cand) && sumPrimeFactors64(cand) == k) {
                a_cache[k] = (seq_t)cand;
                break;
            }
            ++cand;
        }
    }

    return a_cache[n];
}

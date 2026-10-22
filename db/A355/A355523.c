/*
 * A355523  Number of distinct differences between adjacent prime indices of
 *          n.
 * https://oeis.org/A355523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct differences between adjacent prime
 *            indices of n (for n ≥ 1), as defined in OEIS A355523
 * Range:     u8 n <= 9764, u16 n <= 9764, u32 n <= 9764, u64 n <= 9764,
 *            u128 n <= 9764, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"

static uint64_t *primes = NULL;
static size_t    primes_cnt = 0;
static idx_t     primes_limit = 0;

/* Ensure that all primes up to `limit` are available in the global array. */
static void ensure_primes_upto(idx_t limit)
{
    if (primes_limit < limit) {
        if (primes != NULL) {
            free(primes);
        }
        build_primes_upto(limit, &primes, &primes_cnt);
        primes_limit = limit;
    }
}

/* Return the 1‑based index of a prime p (assumes p is prime and ≤ primes_limit). */
static idx_t prime_index(uint64_t p)
{
    for (size_t i = 0; i < primes_cnt; ++i) {
        if (primes[i] == p) {
            return (idx_t)(i + 1);
        }
        if (primes[i] > p) {
            break;
        }
    }
    return (idx_t)0; /* should never happen for valid prime input */
}

seq_t a(idx_t n)
{
    if (n < 2) {               /* n = 1 has no prime indices */
        return (seq_t)0;
    }

    /* primes up to n are required for index lookup */
    ensure_primes_upto(n);

    /* Temporary storage for indices – at most log₂(n) entries for n ≤ 10⁶ */
    seq_t indices[64];
    size_t idx_len = 0;

    uint64_t rem = (uint64_t)n;

    /* Factorize `n` using trial division */
    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= rem; ++i) {
        uint64_t p = primes[i];
        if (rem % p == 0) {
            idx_t k = prime_index(p);
            while (rem % p == 0) {
                rem /= p;
                indices[idx_len++] = k;
            }
        }
    }
    if (rem > 1) {                    /* remaining prime factor > sqrt(n) */
        idx_t k = prime_index(rem);
        indices[idx_len++] = k;
    }

    if (idx_len < 2) {                 /* fewer than two indices → no differences */
        return (seq_t)0;
    }

    /* Compute distinct differences between consecutive indices */
    seq_t diffs[64];
    size_t diffs_len = 0;

    for (size_t i = 1; i < idx_len; ++i) {
        seq_t d = indices[i] - indices[i - 1];
        bool seen = false;
        for (size_t j = 0; j < diffs_len; ++j) {
            if (diffs[j] == d) {
                seen = true;
                break;
            }
        }
        if (!seen) {
            diffs[diffs_len++] = d;
        }
    }

    return (seq_t)diffs_len;
}

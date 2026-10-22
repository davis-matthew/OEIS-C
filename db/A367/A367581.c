/*
 * A367581  Sum of the multiset multiplicity kernel (in which each
 *          multiplicity becomes the least element of that multiplicity) of
 *          the prime indices of n.
 * https://oeis.org/A367581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the multiset multiplicity kernel of the * prime
 *            indices of n. Each prime factor p^e of n contributes e * copies
 *            of the index of p. For every distinct multiplicity k, * the
 *            kernel contains the smallest index among those with multiplicity
 *            k, * repeated |S| times, where S is that set o...
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"

/* Cached list of primes for index lookup */
static uint64_t *prime_list = NULL;
static size_t prime_list_len = 0;
static idx_t prime_list_limit = 0;

/* Ensure that prime_list contains all primes up to at least 'limit'. */
static void ensure_primes_upto(idx_t limit) {
    if ((idx_t)prime_list_limit >= limit) return;
    uint64_t *new_list = NULL;
    size_t new_len = 0;
    build_primes_upto(limit, &new_list, &new_len);
    if (prime_list) free(prime_list);
    prime_list = new_list;
    prime_list_len = new_len;
    prime_list_limit = limit;
}

/* Return the 1‑based index of a prime p (assumes p is prime). */
static uint64_t prime_index_uint64(uint64_t p) {
    ensure_primes_upto(p);
    size_t lo = 0, hi = prime_list_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    return (uint64_t)(lo + 1);   /* convert to 1‑based */
}

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;

    /* Factor n: primes and exponents */
    uint64_t fpr[64];
    uint64_t fex[64];
    size_t fcnt;
    factor_u64((uint64_t)n, fpr, fex, &fcnt);

    if (fcnt == 0) return (seq_t)0;

    /* Arrays indexed by exponent (1 … 64). */
    seq_t min_idx[65];
    uint64_t cnt[65];
    for (int i = 1; i <= 64; ++i) {
        min_idx[i] = (seq_t)(-1);  /* sentinel: max value for unsigned */
        cnt[i] = 0;
    }

    /* Gather counts and minimum indices for each exponent. */
    for (size_t i = 0; i < fcnt; ++i) {
        uint64_t p   = fpr[i];
        uint64_t e   = fex[i];
        uint64_t idx = prime_index_uint64(p);
        cnt[e] += 1;
        if (idx < (uint64_t)min_idx[e]) {
            min_idx[e] = (seq_t)idx;
        }
    }

    /* Compute the kernel sum. */
    seq_t result = (seq_t)0;
    for (int e = 1; e <= 64; ++e) {
        if (cnt[e] > 0) {
            result += (seq_t)cnt[e] * min_idx[e];
        }
    }
    return result;
}

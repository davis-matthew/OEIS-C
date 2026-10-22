/*
 * A363625  Reverse-weighted alternating sum of the integer partition with
 *          Heinz number n.
 * https://oeis.org/A363625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{i=1..k} (-1)^{k-i} * i * y_{k-i+1} where (y1..yk) is
 *            the partition of n with Heinz number n (y_j = index of prime
 *            factor in the prime sequence)
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* Helper to find the 1‑based index of a prime in the primes array. */
static size_t
prime_index(uint64_t p, uint64_t *primes, size_t count)
{
    size_t lo = 0, hi = count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] == p) return mid + 1;       /* 1‑based */
        if (primes[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    return 0;  /* should not happen for a valid prime */
}

/* Cached primes up to the largest value requested so far. */
static uint64_t *cached_primes = NULL;
static size_t    cached_cnt   = 0;
static idx_t    cached_limit  = 0;

static void
ensure_primes_upto(idx_t limit)
{
    if (limit <= cached_limit) return;
    if (cached_primes) free(cached_primes);
    build_primes_upto(limit, (uint64_t **)&cached_primes, &cached_cnt);
    cached_limit = limit;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;               /* empty partition */

    /* ensure we have all primes up to n for prime indexing */
    ensure_primes_upto(n);

    /* temporary array for partition parts, max length ≈ log₂(n) ≤ 64 */
    uint64_t parts[64];
    size_t parts_len = 0;

    idx_t m = n;
    /* factor n using trial division over the cached primes array    */
    for (size_t pi = 0; pi < cached_cnt && (uint64_t)cached_primes[pi] * (uint64_t)cached_primes[pi] <= (uint64_t)m; ++pi) {
        uint64_t p = cached_primes[pi];
        while (m % p == 0) {
            parts[parts_len++] = prime_index(p, cached_primes, cached_cnt);
            m /= p;
        }
    }
    if (m > 1) { /* remaining factor is prime > sqrt(n) */
        parts[parts_len++] = prime_index((uint64_t)m, cached_primes, cached_cnt);
    }

    /* sort parts in non‑increasing order (descending) */
    if (parts_len > 1) {
        for (size_t i = 1; i < parts_len; ++i) {
            uint64_t key = parts[i];
            size_t j = i;
            while (j > 0 && parts[j - 1] < key) {
                parts[j] = parts[j - 1];
                --j;
            }
            parts[j] = key;
        }
    }

    /* compute the reverse‑weighted alternating sum */
    int64_t sum = 0;
    for (size_t j = 0; j < parts_len; ++j) {
        int64_t sign   = (j % 2 == 0) ? 1 : -1;
        int64_t weight = (int64_t)(parts_len - j);
        sum += sign * weight * (int64_t)parts[j];
    }
    return (seq_t)sum;
}

/*
 * A346399  a(n) is the number of symmetrically distributed consecutive primes
 *          centered at n (including n itself if n is prime).
 * https://oeis.org/A346399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of consecutive primes symmetrically distributed
 *            around n (Goldbach pairs of 2n). The algorithm uses a cached
 *            sieve of primes up to 2n+100; it then walks outward from the
 *            primes bracketing n, counting symmetric pairs whose sum is 2n.
 *            This is a direct search rather than a closed‑form...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

/* Cache of primes, shared between calls. */
static uint64_t *cached_primes = NULL;   /* array of primes */
static size_t    cached_cnt   = 0;        /* number of primes cached */
static idx_t     cached_limit = 0;        /* upper bound of cached primes */

/* Ensure primes are cached up to 'limit' (inclusive). */
static void
ensure_primes(idx_t limit)
{
    if (cached_primes != NULL && cached_limit >= limit)
        return;                       /* already sufficient */

    uint64_t *new_primes;
    size_t    new_cnt;
    build_primes_upto(limit, &new_primes, &new_cnt);

    if (cached_primes != NULL)
        free(cached_primes);

    cached_primes = new_primes;
    cached_cnt    = new_cnt;
    cached_limit  = limit;
}

/* Binary search: first index where primes[idx] >= key.  The result
   may equal cached_cnt if no such prime exists. */
static size_t
lower_bound_idx(const uint64_t *arr, size_t sz, idx_t key)
{
    size_t lo = 0, hi = sz;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (arr[mid] < (uint64_t)key)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Primes up to 2*n+100 should be enough for all symmetric pairs. */
    ensure_primes(2 * n + 100);

    /* Find first prime >= n. */
    size_t idx = lower_bound_idx(cached_primes, cached_cnt, n);
    bool n_is_prime = (idx < cached_cnt && cached_primes[idx] == (uint64_t)n);

    /* Determine left/right indices around n. */
    size_t left, right;
    if (n_is_prime) {
        left  = idx;
        right = idx;
        /* Start with the central prime counted once. */
        size_t count = 1;
        /* Extend outward while symmetric pairs sum to 2*n. */
        while (left > 0 && right + 1 < cached_cnt) {
            size_t l = left - 1;
            size_t r = right + 1;
            uint64_t sum = cached_primes[l] + cached_primes[r];
            if (sum == (uint64_t)(2 * n)) {
                count += 2;
                left = l;
                right = r;
            } else
                break;
        }
        return (seq_t)count;
    } else {
        if (idx == 0) /* no prime <= n */
            return (seq_t)0;

        left  = idx - 1;   /* largest prime < n */
        right = idx;       /* smallest prime > n */

        uint64_t sum = cached_primes[left] + cached_primes[right];
        if (sum != (uint64_t)(2 * n))
            return (seq_t)0;   /* no initial symmetric pair */

        size_t count = 2;   /* we have p_left and p_right */
        /* Extend outward as far as possible. */
        while (left > 0 && right + 1 < cached_cnt) {
            size_t l = left - 1;
            size_t r = right + 1;
            if (cached_primes[l] + cached_primes[r] == (uint64_t)(2 * n)) {
                count += 2;
                left = l;
                right = r;
            } else
                break;
        }
        return (seq_t)count;
    }
}

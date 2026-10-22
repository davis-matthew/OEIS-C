/*
 * A359416  Write n as 2^m - k, where 2^m is the least power of 2 >= n (0 <= k
 *          <= 2^(m-1)-1). For n a power of 2 (k = 0), a(n) = n. For numbers
 *          with k > 0, a(n) is the least p*a(k) which has not occurred
 *          previously, the count of k being taken from right to left
 *          (backwards) from k = 1 at 2^m - 1.
 * https://oeis.org/A359416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the recursive rule of OEIS A359416. The
 *            algorithm follows the blockwise backward processing described in
 *            the entry: For each block [2^(m‑1)+1, 2^m‑1] we first compute
 *            the top odd index 2^m‑1, then all even indices in descending
 *            order, and finally all remaining odd indices in de...
 * Range:     u8 n <= 36, u16 n <= 548, u32 n <= 9052, u64 n <= 9052,
 *            u128 n <= 9052, big any n
 * Helpers:   ceil_pow2, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* required: a(idx_t n) returns a(n) for n ≥ 1.  */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/ceil_pow2.h"

static seq_t  *cache      = NULL;
static size_t  cache_cap  = 0;
static size_t  cache_len  = 0;

static seq_t  *used       = NULL;
static size_t  used_cap   = 0;
static size_t  used_len   = 0;

static uint64_t *primes   = NULL;
static size_t   primes_cap = 0;
static size_t   primes_len = 0;
static uint64_t prime_candidate = 3ULL;   /* next odd to test; 2 is already handled */

static void grow_seq_arr(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap >= needed) return;
    size_t newcap = (*cap == 0) ? 8 : *cap << 1;
    if (newcap < needed) newcap = needed;
    seq_t *tmp = (seq_t *)realloc(*arr, newcap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    *arr = tmp;
    *cap = newcap;
}

static void grow_used_arr(size_t needed)
{
    if (used_cap >= needed) return;
    size_t newcap = (used_cap == 0) ? 8 : used_cap << 1;
    if (newcap < needed) newcap = needed;
    seq_t *tmp = (seq_t *)realloc(used, newcap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    used = tmp;
    used_cap = newcap;
}

static void grow_prime_arr(size_t needed)
{
    if (primes_cap >= needed) return;
    size_t newcap = (primes_cap == 0) ? 8 : primes_cap << 1;
    if (newcap < needed) newcap = needed;
    uint64_t *tmp = (uint64_t *)realloc(primes, newcap * sizeof(uint64_t));
    if (!tmp) exit(EXIT_FAILURE);
    primes = tmp;
    primes_cap = newcap;
}

static void add_next_prime(void)
{
    while (true) {
        if (is_prime(prime_candidate)) {
            grow_prime_arr(primes_len + 1);
            primes[primes_len++] = prime_candidate;
            prime_candidate += 2ULL;
            break;
        }
        prime_candidate += 2ULL;
    }
}

static bool value_used(seq_t v)
{
    for (size_t i = 0; i < used_len; ++i)
        if (used[i] == v) return true;
    return false;
}

/* Ensure cache holds values up to n (inclusive) and populates them in correct order. */
static void ensure_cache(idx_t n)
{
    if (n <= 0 || n <= cache_len) return;

    while (cache_len < (size_t)n) {
        idx_t cur = (idx_t)(cache_len + 1);

        /* Special case: power of two. */
        if ((cur & (cur - 1)) == 0) {
            seq_t val = (seq_t)cur;
            grow_seq_arr(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = val;
            grow_used_arr(used_len + 1);
            used[used_len++] = val;
            continue;
        }

        /* Handle the whole block that contains cur. */
        idx_t pow2 = ceil_pow2(cur);
        idx_t high = pow2 - 1;                   /* 2^m - 1  (odd) */
        idx_t low  = (pow2 >> 1) + 1;            /* 2^(m-1)+1 */

        /* Ensure storage for the block. */
        grow_seq_arr(&cache, &cache_cap, (size_t)high);
        grow_used_arr(used_len + (high - low + 1));

        /* 1. Process the top odd index (high). */
        seq_t val_high;
        {
            /* k = 1 for high. a(1)=1. */
            seq_t a_k = 1;
            for (size_t pi = 0; ; ++pi) {
                if (pi == primes_len) add_next_prime();
                uint64_t p = primes[pi];
                seq_t cand = (seq_t)p * a_k;
                if (!value_used(cand)) {
                    val_high = cand;
                    break;
                }
            }
        }
        cache[high - 1] = val_high;
        used[used_len++] = val_high;

        /* 2. Process even indices in descending order. */
        for (idx_t i = high - 1; i >= low; --i) {
            if ((i & 1) == 0) {
                seq_t val_even = (seq_t)2 * cache[(i >> 1) - 1];
                cache[i - 1] = val_even;
                used[used_len++] = val_even;
            }
            if (i == low) break;
        }

        /* 3. Process remaining odd indices in descending order. */
        for (idx_t i = high - 2; i >= low; --i) {
            if ((i & 1) != 0) {
                idx_t k = pow2 - i;                  /* 1 <= k <= 2^(m-1)-1 */
                seq_t a_k = cache[k - 1];
                seq_t val_odd;
                for (size_t pi = 0; ; ++pi) {
                    if (pi == primes_len) add_next_prime();
                    uint64_t p = primes[pi];
                    seq_t cand = (seq_t)p * a_k;
                    if (!value_used(cand)) {
                        val_odd = cand;
                        break;
                    }
                }
                cache[i - 1] = val_odd;
                used[used_len++] = val_odd;
            }
            if (i == low) break;
        }
        cache_len = (size_t)high;  /* all indices up to high are now filled */
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety for undefined case */
    ensure_cache(n);
    return cache[n - 1];
}

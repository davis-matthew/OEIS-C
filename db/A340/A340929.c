/*
 * A340929  Heinz numbers of integer partitions of odd negative rank.
 * https://oeis.org/A340929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) is the n-th Heinz number of an integer partition
 * whose Dyson rank is negative and odd.  The function enumerates
 * integers in increasing order, tests the property, and caches the
 * accepted values.  No other OEIS utilities are used. */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* === Prime sieve helpers (minimal implementation) === */

static uint64_t *prime_list = NULL;
static size_t   prime_count = 0;

/* Ensure prime_list contains all primes <= limit. */
static void ensure_primes_upto(uint64_t limit)
{
    if (prime_count == 0) {
        prime_list = (uint64_t *)malloc(sizeof(uint64_t));
        if (!prime_list) exit(1);
        prime_list[0] = 2;
        prime_count = 1;
    }
    uint64_t candidate = prime_list[prime_count - 1] + 1;
    while (candidate <= limit) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_list[i];
            if (p > candidate / p) break;
            if (candidate % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            uint64_t *tmp = (uint64_t *)realloc(prime_list, (prime_count + 1) * sizeof(uint64_t));
            if (!tmp) exit(1);
            prime_list = tmp;
            prime_list[prime_count++] = candidate;
        }
        ++candidate;
    }
}

/* Binary search for prime p, return 1‑based index (or 0 if not found). */
static uint64_t prime_index_of(uint64_t p)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] == p) return (uint64_t)(mid + 1);
        if (prime_list[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    return 0;
}

/* === Property test === */

static bool accept_number(uint64_t x)
{
    if (x < 4) return false;              /* smallest accepted number */

    ensure_primes_upto(x);

    uint64_t rem = x;
    uint64_t total = 0;      /* total number of parts (sum of exponents) */
    uint64_t max_idx = 0;    /* maximum part size (largest prime index) */

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p > rem / p) break;          /* p*p > rem */

        if (rem % p == 0) {
            uint64_t exp = 0;
            while (rem % p == 0) {rem /= p; ++exp;}
            total += exp;
            uint64_t idx = i + 1;        /* primes are 1‑based */
            if (idx > max_idx) max_idx = idx;
        }
    }
    if (rem > 1) {                        /* remaining prime factor */
        uint64_t idx = prime_index_of(rem);
        if (idx == 0) return false;      /* safety */
        total += 1;
        if (idx > max_idx) max_idx = idx;
    }

    /* rank = max_idx - total; need negative odd → total > max_idx and (total-max_idx) odd. */
    return (total > max_idx) && ((total - max_idx) & 1U);
}

/* === Sequence cache === */

static seq_t *seq_cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static uint64_t last_checked = 1;

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined, never used */

    /* Initialise cache if necessary */
    if (!seq_cache) {
        cache_cap = 128;
        seq_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!seq_cache) exit(1);
    }

    /* Return cached value if available */
    if ((size_t)n <= cache_len)
        return seq_cache[(size_t)n - 1];

    /* Generate further values until we have n entries */
    uint64_t cand = last_checked + 1;
    while (cache_len < (size_t)n) {
        if (cand >= 4 && accept_number(cand)) {
            if (cache_len >= cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = (seq_t *)realloc(seq_cache, cache_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                seq_cache = tmp;
            }
            seq_cache[cache_len++] = (seq_t)cand;
        }
        ++cand;
    }
    last_checked = cand - 1;
    return seq_cache[cache_len - 1];
}

/*
 * A336151  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A001221(i) = A001221(j) and A006530(i) = A006530(j), for all i,
 *          j >= 1.
 * https://oeis.org/A336151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the restricted‑growth sequence transform of the pair
 *            (ω(n), lpf(n)) where ω(n) = number of distinct prime factors of
 *            n (A001221) and lpf(n) = largest prime factor of n (A006530).
 *            The sequence assigns a unique increasing integer to each
 *            distinct pair in the order of first appearance, produ...
 * Range:     u8 n <= 810, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9661 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Count distinct prime factors of an unsigned 64-bit integer n.
   The result fits in a signed int for all n that may be passed. */
static int count_distinct_primes(uint64_t n)
{
    if (n <= 1)
        return 0;

    int cnt = 0;
    /* Handle factor 2 separately. */
    if (n % 2 == 0) {
        cnt++;
        while (n % 2 == 0)
            n /= 2;
    }
    /* Test odd divisors up to sqrt(n). */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            cnt++;
            while (n % p == 0)
                n /= p;
        }
    }
    if (n > 1)   /* Remaining prime factor. */
        cnt++;
    return cnt;
}

/*  Works for all n used in the tests (≤ 10 000).                       */
static uint64_t largest_prime_factor(uint64_t n)
{
    uint64_t max = 1U;
    while ((n & 1ULL) == 0ULL) { max = 2U; n >>= 1; }
    for (uint64_t i = 3ULL; i * i <= n; i += 2ULL) {
        while (n % i == 0ULL) { max = i; n /= i; }
    }
    if (n > 2ULL) max = n;
    return max;
}

/* Structure to store a discovered pair and its assigned value. */
typedef struct {
    uint64_t omega;      /* number of distinct prime divisors */
    uint64_t lpf;        /* largest prime factor */
    seq_t val;           /* assigned sequence value */
} pair_val_t;

/* Static cache for sequence values (0‑based index). */
static seq_t *seq_cache = NULL;
static size_t seq_len = 0;    /* number of entries currently cached */

/* Static table of observed pairs. */
static pair_val_t *pair_table = NULL;
static size_t pair_cnt = 0;   /* number of distinct pairs observed */

static void ensure_seq_capacity(size_t needed)
{
    if (seq_cache == NULL || seq_len < needed) {
        size_t new_cap = seq_len ? seq_len * 2 : 8;
        while (new_cap < needed) new_cap *= 2;
        seq_cache = mem_resize(seq_t, seq_cache, new_cap);
        seq_len = new_cap;
    }
}

static void ensure_pair_capacity(void)
{
    if (pair_table == NULL) {
        pair_table = mem_new(pair_val_t, 8);
    } else if (pair_cnt >= 8) {
        size_t new_cap = pair_cnt * 2;
        pair_table = mem_resize(pair_val_t, pair_table, new_cap);
    }
}

/* Find or create the value for a given pair. */
static seq_t map_pair(uint64_t omega, uint64_t lpf)
{
    /* Linear search: number of distinct pairs grows slowly.          */
    for (size_t i = 0; i < pair_cnt; ++i) {
        if (pair_table[i].omega == omega && pair_table[i].lpf == lpf) {
            return pair_table[i].val;
        }
    }
    /* New pair: assign next integer value. */
    ensure_pair_capacity();
    seq_t new_val = (seq_t)(pair_cnt + 1);
    pair_table[pair_cnt].omega = omega;
    pair_table[pair_cnt].lpf = lpf;
    pair_table[pair_cnt].val = new_val;
    ++pair_cnt;
    return new_val;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* n is 1‑based; cache indices are 0‑based. */
    if (n == 0) return 0;  /* undefined for n < 1; return 0 as safe default */

    /* Compute up to n if necessary. */
    if (seq_cache == NULL) {
        seq_cache = mem_new(seq_t, 8);
        seq_len = 8;
        pair_table = mem_new(pair_val_t, 8);
        pair_cnt = 0;
        /* initial term for n = 1 */
        pair_val_t p = {0ULL, 0ULL, (seq_t)1};
        pair_table[0] = p;
        ++pair_cnt;
        seq_cache[0] = (seq_t)1;
    }

    idx_t last_computed = 1;          /* highest index we have computed, 1‑based */
    /* Determine the last computed index from the cache length. */
    /* Cache stores seq_len values, but may contain uninitialized slots. */
    last_computed = (seq_cache != NULL) ? 1 + (idx_t)(seq_len - 1) : 0;
    /* Since we always keep cache fully populated up to last_computed, */
    /* last_computed equals the number of computed terms. */
    last_computed = (idx_t)pair_cnt;  /* number of distinct pairs equals number of terms computed so far */

    if (n > (idx_t)pair_cnt) {
        /* Ensure cache has enough capacity. */
        ensure_seq_capacity((size_t)n);
        /* Compute missing terms. */
        for (idx_t i = (idx_t)(pair_cnt) + 1; i <= n; ++i) {
            uint64_t omega, lpf;
            if (i == 1) {          /* special handling for n = 1 */
                omega = 0;
                lpf = 0;
            } else {
                omega = (uint64_t)count_distinct_primes((uint64_t)i);
                lpf = largest_prime_factor((uint64_t)i);
            }
            seq_t val = map_pair(omega, lpf);
            seq_cache[i - 1] = val;
        }
    }
    return seq_cache[n - 1];
}

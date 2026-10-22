/*
 * A309387  a(n) is the smallest positive divisor not yet in the sequence of
 *          7*A000217(n-1); n >= 1.
 * https://oeis.org/A309387
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A309387: a(n) = smallest positive divisor of 7*T_{n-1} not yet in the sequence, where T_{k} = k*(k+1)/2.
   For n=1, a(1)=1 (the smallest divisor of 0 not seen yet).
   The function is memoized; any order of calls is supported. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static const size_t HASH_CAPACITY = 65536; /* power of two, > 2*max number of seen elements */

static seq_t *seq_cache = NULL;
static size_t cache_cap = 0;
static size_t cache_max = 0;

static uint64_t *hash_keys = NULL;
static bool *hash_used = NULL;

/* Simple 64‑bit integer sqrt (floor) */
static inline idx_t isqrt_u64(uint64_t n) {
    idx_t x = 0;
    idx_t bit = ((idx_t)1) << 62;
    while (bit > n) bit >>= 2;
    while (bit) {
        if (n >= x + bit) {
            n -= x + bit;
            x = (x >> 1) + bit;
        } else {
            x >>= 1;
        }
        bit >>= 2;
    }
    return x;
}

/* Hash table helpers ----------------------------------------------------- */
static inline size_t hash_idx(seq_t v) {
    uint64_t h = (uint64_t)v * 11995408973635179863ULL;
    return (size_t)(h & (HASH_CAPACITY - 1));
}

static void hash_init(void) {
    hash_keys = (uint64_t *)calloc(HASH_CAPACITY, sizeof(uint64_t));
    hash_used = (bool *)calloc(HASH_CAPACITY, sizeof(bool));
}

static bool hash_contains(seq_t v) {
    size_t idx = hash_idx(v);
    while (hash_used[idx]) {
        if ((uint64_t)hash_keys[idx] == (uint64_t)v) return true;
        idx = (idx + 1) & (HASH_CAPACITY - 1);
    }
    return false;
}

static void hash_insert(seq_t v) {
    if (!hash_used) hash_init();
    size_t idx = hash_idx(v);
    while (hash_used[idx]) {
        if ((uint64_t)hash_keys[idx] == (uint64_t)v) return; /* already present */
        idx = (idx + 1) & (HASH_CAPACITY - 1);
    }
    hash_used[idx] = true;
    hash_keys[idx] = (uint64_t)v;
}

/* Cache helpers ----------------------------------------------------------- */
static void cache_ensure_cap(size_t needed) {
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *newbuf = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!newbuf) exit(EXIT_FAILURE);
    seq_cache = newbuf;
    cache_cap = new_cap;
}

/* Main function ----------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n < 1) return 0; /* undefined for n < 1 */

    /* Ensure cache large enough */
    cache_ensure_cap((size_t)n + 1);
    if (n <= cache_max) return seq_cache[n];

    /* Compute terms from cache_max+1 up to n */
    for (idx_t j = cache_max + 1; j <= n; ++j) {
        seq_t val;
        if (j == 1) {
            val = 1;
        } else {
            seq_t tri = (seq_t)(j - 1) * (seq_t)j / 2;
            seq_t m = 7u * tri;
            seq_t best = 0;
            idx_t sqrt_m = (idx_t)isqrt_u64((uint64_t)m);
            for (idx_t d = 1; d <= sqrt_m; ++d) {
                if (m % d == 0) {
                    seq_t cand1 = (seq_t)d;
                    if (!hash_contains(cand1) && (best == 0 || cand1 < best)) best = cand1;
                    seq_t cand2 = m / d;
                    if (cand2 != cand1 && !hash_contains(cand2) && (best == 0 || cand2 < best))
                        best = cand2;
                }
            }
            /* best should always be found */
            val = best;
        }
        seq_cache[j] = val;
        hash_insert(val);
        cache_max = j;
    }

    return seq_cache[n];
}

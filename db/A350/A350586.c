/*
 * A350586  Numbers m with exactly 2 groups of order m, where one is abelian
 *          and the other is nonabelian.
 * https://oeis.org/A350586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 8581, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) enumerates the positive integers m having exactly
 * two distinct groups of order m, namely the cyclic (abelian) group and a
 * single non‑abelian group.  The sequence is produced by testing square‑free
 * integers m, checking that they have exactly one pair of distinct
 * prime divisors (p , q) with p < q and p | (q‑1).  Numbers with this
 * property are generated in increasing order and cached for subsequent
 * calls.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"   /* provides: factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt) */

/* static cache for previously computed values */
static seq_t *cache = NULL;          /* dynamic array of terms */
static size_t cache_size = 0;        /* number of terms stored */
static size_t cache_cap   = 0;        /* current allocated capacity */
static uint64_t next_candidate = 2;   /* next integer to test */

/* grow the cache if necessary */
static void ensure_cache_cap(size_t needed) {
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) /* allocation failed – propagate */
        abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* test whether a square‑free integer m has exactly one prime pair (p<q)
 * with p dividing (q‑1); if so, m appears in the sequence.
 */
static bool is_valid_order(uint64_t m) {
    /* factor m */
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_u64(m, pr, ex, &cnt);

    /* m must be square‑free */
    for (size_t i = 0; i < cnt; ++i)
        if (ex[i] != 1) return false;

    if (cnt < 2)
        return false;  /* at most one prime – only one group exists */

    /* count pairs (p<q) with p | (q-1) */
    size_t pair_count = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        for (size_t j = i + 1; j < cnt; ++j) {
            uint64_t q = pr[j];
            if ((q % p) == 1)
                ++pair_count;
            if (pair_count > 1)          /* more than one such pair → >2 groups */
                return false;
        }
    }
    /* exactly one pair yields exactly two groups (abelian + one non‑abelian) */
    return pair_count == 1;
}

/* public sequence function */
seq_t a(idx_t n) {
    /* n is 1‑based; grow cache to contain at least n terms */
    while (cache_size < (size_t)n) {
        if (is_valid_order(next_candidate)) {
            ensure_cache_cap(cache_size + 1);
            cache[cache_size++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return cache[(size_t)n - 1];
}

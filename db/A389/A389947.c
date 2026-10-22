/*
 * A389947  Squares of nonsquarefree numbers k that are not squareful.
 * https://oeis.org/A389947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 74, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A389947
 * Squares of nonsquarefree numbers k that are not squareful.
 * Implementation: generate numbers k in increasing order that satisfy
 *    (∃ prime p with exp(p) ≥ 2)  ∧  (∃ prime q with exp(q) = 1)
 * and return a(n) = k^2.
 * The routine uses factor_u64() from the utils library to test each k.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"

/* Check if k is nonsquarefree but not squareful */
static bool is_valid(uint64_t k)
{
    if (k <= 1)
        return false;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64(k, primes, exps, &cnt);

    bool has_one = false, has_square = false;
    for (size_t i = 0; i < cnt; i++) {
        if (exps[i] == 1)
            has_one = true;
        else if (exps[i] >= 2)
            has_square = true;
        if (has_one && has_square)
            return true;
    }
    return false;
}

/* Memoised sequence storage */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;
static idx_t current_k = 2;   /* next candidate k to test */

/* Ensure capacity of the cache array */
static void ensure_cache_capacity(idx_t needed)
{
    if (cache_len + 1 > cache_cap) {
        size_t new_cap = cache_cap == 0 ? 4 : cache_cap * 2;
        seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = p;
        cache_cap = new_cap;
    }
}

/* Main function: a(n) for the OEIS sequence */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined for n=0, safe return */

    /* Build cache up to required index */
    while (cache_len < n) {
        uint64_t k = current_k;
        current_k++;

        if (is_valid(k)) {
            seq_t val = (seq_t)k * (seq_t)k; /* square, may overflow seq_t */
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = val;
        }
    }

    return cache[n - 1];   /* 1‑based indexing */
}

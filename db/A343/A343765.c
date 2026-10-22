/*
 * A343765  Lexicographically earliest sequence of distinct nonprime numbers
 *          such that for any n > 0, a(n+1) = a(n) XOR 2^k for some k >= 0 as
 *          small as possible (where XOR denotes the bitwise XOR operator).
 * https://oeis.org/A343765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
 *   a(1) = 1
 *   For n ≥ 1, a(n+1) is the smallest value obtainable by
 *   a(n) XOR 2^k (k ≥ 0) that is a non‑prime integer not already in
 *   the sequence.
 * The function implements this rule and memoises the generated terms.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h" /* is_prime_big(seq_t) */
#include "../utils/pow2.h"          /* pow2(idx_t) → 2^exp as seq_t */

static seq_t *cache = NULL;        /* array of generated terms */
static idx_t  cache_len = 0;        /* number of terms stored */
static idx_t  cache_cap = 0;        /* allocated capacity */

/* Ensure the cache can hold at least `need` terms (1‑based). */
static void ensure_cache_cap(idx_t need)
{
    if (need <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < need)
        new_cap <<= 1;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Check whether a candidate value is already in the cache. */
static bool used_in_cache(const seq_t *cand)
{
    for (idx_t i = 0; i < cache_len; ++i)
        if (cache[i] == *cand)
            return true;
    return false;
}

/* Compute the next term from the current term. */
static seq_t next_term(const seq_t *prev)
{
    for (idx_t k = 0;; ++k) {
        seq_t mask = pow2(k);
        seq_t cand = (*prev) ^ mask;
        if (cand == 0)                  /* avoid 0; sequence consists of positive integers */
            continue;
        if (is_prime_big(cand))         /* must be non‑prime */
            continue;
        if (used_in_cache(&cand))       /* must be distinct */
            continue;
        return cand;
    }
}

/* Public accessor: return a(n) for n ≥ 1 (undefined for n < 1). */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                      /* safety for out‑of‑range indices */

    /* Initialise cache with a(1) if it is empty. */
    if (cache_len == 0) {
        ensure_cache_cap(1);
        cache[0] = 1;                  /* a(1) = 1 */
        cache_len = 1;
    }

    /* Generate terms up to the requested index. */
    while (cache_len < n) {
        seq_t nxt = next_term(&cache[cache_len - 1]);
        ensure_cache_cap(cache_len + 1);
        cache[cache_len] = nxt;
        ++cache_len;
    }

    return cache[n - 1];
}

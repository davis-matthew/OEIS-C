/*
 * A245727  Least number k >= 0 such that n concatenated with n + k is prime.
 * https://oeis.org/A245727
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k ≥ 0 such that concat(n, n+k) is prime
 * Range:     u8 n <= 8, u16 n <= 83, u32 n <= 83, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   concat_parts, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/concat_parts.h"
#include "../utils/is_prime_big.h"

static seq_t *cache = NULL;      /* cached a(n) values (0‑based index) */
static bool *seen  = NULL;      /* true if cache[i] is valid */
static idx_t cache_cap = 0;     /* current capacity of cache tables */

/* Ensure the tables can hold indices up to (needed‑1). */
static void ensure_capacity(idx_t needed) {
    if (needed <= cache_cap)
        return;
    idx_t new_cap = (needed < 1024 ? 1024 : needed * 2);
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    bool *new_seen  = (bool *)realloc(seen,  new_cap * sizeof(bool));
    if (!new_cache || !new_seen)
        return;                       /* allocation failed – leave NULL */
    /* mark the newly allocated slots as unused */
    for (idx_t i = cache_cap; i < new_cap; ++i)
        new_seen[i] = false;
    cache = new_cache;
    seen  = new_seen;
    cache_cap = new_cap;
}

/* Public OEIS function. */
seq_t a(idx_t n) {
    /* n is 1‑based per OEIS offset; we store at cache[n] */
    idx_t idx = n;
    ensure_capacity(idx + 1);          /* need at least n+1 slots */

    if (seen[idx])
        return cache[idx];

    /* Search for the smallest k such that concat(n, n+k) is prime. */
    for (idx_t k = 0; ; ++k) {
        idx_t candidate = n + k;
        seq_t num = concat_parts(n, candidate);
        if (is_prime_big(num)) {
            seq_t result = (seq_t)k;
            cache[idx] = result;
            seen[idx] = true;
            return result;
        }
    }
}

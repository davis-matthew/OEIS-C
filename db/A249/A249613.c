/*
 * A249613  Integers that reach the (1023, 512) cycle described in A234534,
 *          after iterations of numerator(sigma(n)/n) = A017665(n).
 * https://oeis.org/A249613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   gcd, seq_realloc, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A249613: numbers that eventually reach the  (1023,512) cycle
   under the iteration  n ↦ numerator(σ(n)/n) = σ(n) / gcd(σ(n), n).
   The function returns the nth positive integer (1‑based index) that
   reaches the cycle, caching previously computed terms for reuse. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/sigma_of.h"
#include "../utils/gcd.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;       /* cached sequence values                */
static size_t cache_len = 0;      /* number of cached terms                */
static size_t cache_cap = 0;      /* allocated capacity                    */
static uint64_t next_to_test = 1; /* next integer to test for membership   */

/* Ensure that the cache has capacity for at least 'needed' elements */
static void ensure_capacity(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap : 16;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *p = seq_realloc(cache, cache_cap, new_cap);
        if (!p) abort();                      /* out of memory */
        cache = p;
        cache_cap = new_cap;
    }
}

/* Perform the σ‑numerator iteration on 'start' (uint64_t).  Return true
   iff the sequence eventually reaches 1023 or 512 before exceeding
   UINT64_MAX or reaching the 2000‑step limit. */
static bool hits_cycle(uint64_t start)
{
    seq_t cur = (seq_t)start;
    for (size_t step = 0; step < 2000; ++step) {
        if (cur == (seq_t)1023 || cur == (seq_t)512) return true;
        if (cur > (seq_t)UINT64_MAX) return false;          /* cannot continue */
        idx_t idx = (idx_t)cur;                            /* safe cast */
        seq_t sigma = sigma_of(idx);
        seq_t g      = gcd(sigma, cur);
        seq_t nxt    = sigma / g;
        cur = nxt;
    }
    return false; /* did not hit the cycle within the limit */
}

/* Main sequence function: returns the nth term (1‑based) of A249613. */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined, but avoid UB */

    /* Grow the cache until it contains at least n terms */
    while (cache_len < (size_t)n) {
        if (hits_cycle(next_to_test)) {
            ensure_capacity(cache_len + 1);
            cache[cache_len++] = (seq_t)next_to_test;
        }
        ++next_to_test;
    }
    return cache[n - 1];
}

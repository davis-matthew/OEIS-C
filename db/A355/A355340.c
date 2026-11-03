/*
 * A355340  a(0) = 0; for n >= 1, a(n) = a(n-1) XOR A001511(n), where XOR
 *          denotes bitwise exclusive-or (A003987) and A001511 is the binary
 *          ruler function.
 * https://oeis.org/A355340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 16383, u16 n <= 16383, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 121 terms through n = 9987 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0; for n >= 1, a(n) = a(n‑1) XOR (trailing zero count of n + 1)
   (i.e. the position of the least significant 1‑bit in n).  The sequence is
   memoised so calls in any order are efficient. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *cache = NULL;    /* memoised values */
static idx_t cache_len = 0;    /* length of filled cache (next free index) */
static idx_t cache_cap = 0;    /* allocated size of cache */

/* Helper to grow the cache to accommodate index n (inclusive). */
static void ensure_cache(idx_t n) {
    if (n < cache_len) return;                   /* already computed */

    /* Grow capacity if necessary */
    if (cache_cap <= n) {
        idx_t new_cap = (cache_cap == 0) ? 1 : cache_cap;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort();                       /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize base case if needed */
    if (cache_len == 0) {
        cache[0] = 0;
        cache_len = 1;
    }

    /* Compute values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* trailing zeros count of i plus one */
        seq_t r = (seq_t)__builtin_ctzll((unsigned long long)i) + 1;
        cache[i] = cache[i - 1] ^ r;
    }
    cache_len = n + 1;
}

seq_t a(idx_t n) {
    ensure_cache(n);
    return cache[n];
}

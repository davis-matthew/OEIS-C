/*
 * A306860  a(n) = Sum_{k=0..floor(n/9)} binomial(n,9*k).
 * https://oeis.org/A306860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..⌊n/9⌋} C(n,9k) Recurrence (valid for all n ≥
 *            9): a(n) = 9*a(n-1) – 36*a(n-2) + 84*a(n-3) – 126*a(n-4) +
 *            126*a(n-5) – 84*a(n-6) + 36*a(n-7) – 9*a(n-8) + 2*a(n-9) Initial
 *            values: a(0)=a(1)=…=a(8)=1 (All arithmetic is performed modulo
 *            the width of seq_t.)
 * Range:     u8 n <= 12, u16 n <= 18, u32 n <= 34, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc.h"

static seq_t *cache = NULL;   /* cache[0..cache_size-1] contains a(0)..a(cache_size-1) */
static size_t cache_size = 0;

/* Ensure that cache contains values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    /* Initial cache of size 9 (a(0)..a(8) = 1). */
    if (cache_size == 0) {
        cache_size = 9;
        cache = seq_alloc(cache_size);
        for (size_t i = 0; i < cache_size; ++i)
            cache[i] = 1;
        return;
    }

    /* Already enough cached? */
    if ((size_t)n + 1 <= cache_size)
        return;

    /* Allocate a larger array and copy existing values */
    size_t old_size = cache_size;
    size_t new_size = (size_t)n + 1;
    seq_t *new_cache = seq_alloc(new_size);
    for (size_t i = 0; i < old_size; ++i)
        new_cache[i] = cache[i];
    /* Replace cache (old memory is intentionally leaked – acceptable for this
       usage pattern and keeps the code small). */
    cache = new_cache;
    cache_size = new_size;

    /* Compute missing terms (from old_size to n, inclusive). */
    for (idx_t i = old_size; i <= n; ++i) {
        if (i < 9) {              /* indices 0..8 are all 1 */
            cache[(size_t)i] = 1;
            continue;
        }

        seq_t a0 = cache[(size_t)(i-1)];
        seq_t a1 = cache[(size_t)(i-2)];
        seq_t a2 = cache[(size_t)(i-3)];
        seq_t a3 = cache[(size_t)(i-4)];
        seq_t a4 = cache[(size_t)(i-5)];
        seq_t a5 = cache[(size_t)(i-6)];
        seq_t a6 = cache[(size_t)(i-7)];
        seq_t a7 = cache[(size_t)(i-8)];
        seq_t a8 = cache[(size_t)(i-9)];

        seq_t pos =  9*a0 + 84*a2 + 126*a4 + 36*a6 + 2*a8;
        seq_t neg = 36*a1 + 126*a3 + 84*a5 + 9*a7;
        cache[(size_t)i] = pos - neg;   /* unsigned subtraction handles wrap‑around */
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}

/*
 * A258056  3x + 1 sequence starting at 75.
 * https://oeis.org/A258056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 75; a(n) = 3*a(n-1)+1 if a(n-1) is odd, a(n) = a(n-1)/2
 *            otherwise
 * Range:     u8 n <= 2, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <assert.h>

/* The cache holds computed values up to CACHE_CAPACITY-1.
   It is safe for both integer and arbitrary‑precision seq_t types. */
#define CACHE_CAPACITY 20000

static seq_t cache[CACHE_CAPACITY];
static idx_t cache_len = 0;

/* Ensure that cache has entries up to index n. */
static void
ensure_cache(idx_t n)
{
    /* The sequence is only used for n up to about 10000;
       we guard against out‑of‑bounds requests. */
    assert(n < CACHE_CAPACITY);

    if (n < cache_len)
        return;

    /* Initialise the very first value. */
    if (cache_len == 0) {
        cache[0] = (seq_t)75;
        cache_len = 1;
    }

    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        if ((prev & (seq_t)1) != 0)
            cache[i] = prev * (seq_t)3 + (seq_t)1;
        else
            cache[i] = prev / (seq_t)2;
    }
    cache_len = n + 1;
}

/* Public function required by the specification. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}

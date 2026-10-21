/*
 * A295016  Squares whose largest digit is 6.
 * https://oeis.org/A295016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A295006(n)^2 (A295006 is the increasing sequence of
 *            non‑negative integers whose decimal digits are all ≤6 and
 *            contain at least one 6). This implementation builds (and
 *            memoises) the sequence of such squares
 * Range:     u8 n <= 3, u16 n <= 50, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Static state for memoisation */
static seq_t *cache = NULL;     /* computed squares (a(n))          */
static idx_t cache_size = 0;    /* number of terms stored           */
static idx_t cache_cap  = 0;    /* allocated capacity of cache      */
static idx_t next_sqrt  = 1;    /* next integer whose square to test */

/* Return true iff x's decimal representation contains
   only digits ≤6 and at least one digit 6. */
static bool largest_digit_is_6(seq_t x)
{
    bool has6 = false;
    while (x > 0) {
        seq_t d = x % 10;
        if (d > 6)
            return false;           /* digit >6 → reject */
        if (d == 6)
            has6 = true;             /* mark presence of 6 */
        x /= 10;
    }
    return has6;                    /* accept only if a 6 is present */
}

/* Extend the cache until it contains at least up_to terms. */
static void ensure_cache(idx_t up_to)
{
    if (up_to <= cache_size)
        return;                              /* already ready */

    if (cache_cap < up_to) {
        /* Allocate at least 64 entries or the requested size,
           whichever is larger. */
        cache_cap = up_to > 64 ? up_to : 64;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }

    while (cache_size < up_to) {
        seq_t sq = (seq_t)next_sqrt * (seq_t)next_sqrt;
        if (largest_digit_is_6(sq))
            cache[cache_size++] = sq;
        ++next_sqrt;
    }
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                               /* undefined for n=0 */
    ensure_cache(n);
    return cache[n - 1];                          /* a(1) is stored at index 0 */
}

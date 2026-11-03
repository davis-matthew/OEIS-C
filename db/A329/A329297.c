/*
 * A329297  Numbers whose digits are in nondecreasing order in bases 7 and 8.
 * https://oeis.org/A329297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 39, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose digits are in nondecreasing order in bases 7 and 8. (A329297) */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Check that the digits of v in the given base are nondecreasing
   from most‑significant to least‑significant. */
static bool nondecreasing_in_base(uint64_t v, unsigned base)
{
    if (v == 0) return true;          /* 0 has a single digit 0 */
    int digits[64];                   /* enough for any 64‑bit number in base ≥ 2 */
    int len = 0;
    while (v > 0 && len < 64) {
        digits[len++] = (int)(v % base);
        v /= base;
    }
    /* digits[0] = LSD, digits[len-1] = MSD */
    int prev = digits[len - 1];
    for (int i = len - 2; i >= 0; --i) {
        int cur = digits[i];
        if (prev > cur) return false; /* decreasing step */
        prev = cur;
    }
    return true;
}

/* Return the n‑th term of the sequence, with offset 1 (a(1)=0). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* undefined, but guard */

    /* Static state for incremental construction. */
    static seq_t *cache   = NULL; /* array of found terms */
    static size_t cap    = 0;     /* current capacity of cache   */
    static size_t size   = 0;     /* number of valid terms stored */
    static idx_t next    = 0;     /* next integer to test         */

    /* Grow the cache until we have at least n terms. */
    while (size < (size_t)n) {
        /* Ensure sufficient storage. */
        if (cache == NULL || size == cap) {
            size_t new_cap = cap ? cap * 2 : 16;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) {            /* allocation failure */
                if (cache) mem_free(cache);
                return 0;
            }
            cache = tmp;
            cap   = new_cap;
        }

        /* Test the next integer. */
        uint64_t v = next++;           /* use 64‑bit to hold the test value */
        if (nondecreasing_in_base(v, 7) && nondecreasing_in_base(v, 8)) {
            cache[size++] = (seq_t) v; /* truncate to seq_t if necessary */
        }
    }

    return cache[n - 1];
}

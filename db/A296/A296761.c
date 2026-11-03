/*
 * A296761  Numbers whose base-16 digits d(m), d(m-1), ..., d(0) have #(rises)
 *          < #(falls); see Comments.
 * https://oeis.org/A296761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 120, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9578 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A296761: numbers whose base‑16 digits d(m)…d(0) have #(rises) < #(falls) */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true iff the hexadecimal representation of x has fewer rises
   than falls.  Equal neighbouring digits are ignored.  Zero has no
   neighbouring digits, so it is false. */
static bool falls_more_than_rises(seq_t x)
{
    if (x == 0) return false;          /* zero has no adjacent digits */

    uint8_t rev[64];        /* digits low → high order (max 64 hex digits) */
    int len = 0;
    seq_t tmp = x;
    while (tmp > 0) {
        rev[len++] = (uint8_t)(tmp % 16);
        tmp /= 16;
    }

    int rises = 0, falls = 0;
    for (int i = len - 1; i > 0; --i) {
        uint8_t high = rev[i];
        uint8_t low  = rev[i - 1];
        if (high < low) rises++;
        else if (high > low) falls++;
        /* equal digits are ignored */
    }
    return rises < falls;
}

/* Dynamic cache of generated terms: CACHE[i] == a(i+1) */
static seq_t *CACHE = NULL;
static size_t CACHE_USED = 0;
static size_t CACHE_CAP  = 0;
static seq_t NEXT = 16;   /* next integer to test */

/* Grow the cache if more space is required.  In C++ we must cast the
   void* returned by realloc. */
static void grow_cache(void)
{
    if (CACHE_CAP < CACHE_USED + 1) {
        size_t new_cap = CACHE_CAP ? CACHE_CAP * 2 : 256;
        if (new_cap < CACHE_USED + 1) new_cap = CACHE_USED + 1;
        CACHE = mem_resize(seq_t, CACHE, new_cap);
        if (!CACHE) abort();          /* allocation failure – terminate */
        CACHE_CAP = new_cap;
    }
}

/* Ensure that CACHE contains at least n terms. */
static void ensure_terms(idx_t n)
{
    while (CACHE_USED < n) {
        while (!falls_more_than_rises(NEXT)) ++NEXT;
        grow_cache();
        CACHE[CACHE_USED++] = NEXT;
        ++NEXT;
    }
}

/* Public function:  return the n‑th term (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, harmless */
    ensure_terms(n);
    return CACHE[n - 1];
}

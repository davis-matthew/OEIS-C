/*
 * A296697  Numbers whose base-5 digits d(m), d(m-1), ... d(0) have #(rises) =
 *          #(falls); see Comments.
 * https://oeis.org/A296697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 106, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A296697: Numbers whose base‑5 digits d(m)…d(0) satisfy
 * #(rises) = #(falls).
 *
 * The routine searches through the natural numbers and collects
 * those that satisfy the condition.  The results are cached
 * in a static array so that repeated calls with arbitrary
 * n are fast.  For n < 1 the function returns 0; the OEIS
 * offset is 1, so a(1) is the first term of the sequence.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* seq_t and idx_t are supplied by the build environment. */

/* ------------------------------------------------------------------ */
/* Test whether a number has equal numbers of rises and falls in base 5 */
/* ------------------------------------------------------------------ */
static bool equal_rises_falls(idx_t x)
{
    /* x > 0 for all calls */
    uint64_t tmp = (uint64_t)x;               /* use 64‑bit temp to avoid overflow */
    uint8_t prev = tmp % 5;                   /* least significant digit */
    tmp /= 5;
    unsigned rises = 0, falls = 0;
    while (tmp) {
        uint8_t cur = tmp % 5;
        if (cur > prev)      rises++;
        else if (cur < prev) falls++;
        prev = cur;
        tmp /= 5;
    }
    return rises == falls;
}

/* ------------------------------------------------------------------ */
/* Grow the static cache to hold at least 'needed' entries             */
/* ------------------------------------------------------------------ */
static void grow_cache(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t new_cap = (*cap > 0) ? (*cap << 1) : 64;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        *arr = tmp;
        *cap = new_cap;
    }
}

/* ------------------------------------------------------------------ */
/* Public function: return the nth term of A296697 (n >= 1)           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* static cache persists across calls */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;          /* number of entries currently stored */
    static idx_t next = 1;          /* next integer to test */

    if (n == 0) return (seq_t)0;    /* undefined for n=0, but harmless */

    /* extend the cache until it contains the n‑th term */
    while (len < n) {
        if (equal_rises_falls(next)) {
            grow_cache(&cache, &cap, len + 1);
            cache[len++] = (seq_t)next;
        }
        next++;
    }

    return cache[n - 1];
}

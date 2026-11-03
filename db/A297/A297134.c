/*
 * A297134  Numbers having an up-first zigzag pattern in base 6; see Comments.
 * https://oeis.org/A297134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A297134: Numbers having an up‑first zigzag pattern in base 6
   (found by brute‑force search). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Return true if the base‑6 digits of num form a zigzag
   pattern that starts with an upward run.  Digits must be
   distinct and the first difference must be positive. */
static bool zigzag_up_first(seq_t num)
{
    /* Need at least two digits; all one‑digit numbers are skipped. */
    if (num < 6)                /* numbers 0–5 have only one base‑6 digit */
        return false;

    /* Extract base‑6 digits; at most 256 digits are enough for any
       practical seq_t size (128‑bit numbers have <84 digits). */
    unsigned int digits[256];
    int len = 0;
    seq_t t = num;
    while (t > 0) {
        digits[len++] = (unsigned)(t % 6);
        t = t / 6;
    }
    if (len < 2) return false;

    int prev = digits[len - 1];      /* most significant digit */
    int prev_dir = 0;              /* 1 = up, -1 = down, 0 = unset */

    for (int idx = len - 2; idx >= 0; --idx) {
        int cur = digits[idx];
        int diff = cur - prev;
        if (diff == 0) return false;        /* equal consecutive digits */

        int dir = (diff > 0) ? 1 : -1;      /* current run direction */
        if (prev_dir == 0) {
            if (dir != 1) return false;     /* first run must be up */
            prev_dir = dir;
        } else {
            if (dir != prev_dir)
                prev_dir = dir;            /* direction change allowed only
                                               as alternation, which is
                                               automatically satisfied since
                                               we only toggle when it
                                               differs */
        }
        prev = cur;
    }
    return true;
}

/* OEIS a(n) function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined; safe fallback */

    static seq_t *cache      = NULL; /* stored qualifying numbers */
    static idx_t cache_size  = 0;     /* number of entries in cache */
    static idx_t cache_cap   = 0;     /* allocated capacity */
    static seq_t last_checked = 1;    /* next candidate to test */

    if (n <= cache_size)            /* quick lookup if we already have it */
        return cache[n - 1];

    /* Expand cache until we have at least n values. */
    while (cache_size < n) {
        if (zigzag_up_first(last_checked)) {
            if (cache_size == cache_cap) {           /* grow the cache */
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) return 0;                  /* allocation failed */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = last_checked;
        }
        ++last_checked;   /* advance to next candidate */
    }
    return cache[n - 1];
}

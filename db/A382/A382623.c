/*
 * A382623  Positive integers that contain an even digit d immediately
 *          preceded by a digit <= d.
 * https://oeis.org/A382623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n-th positive integer containing an even digit d
 *            immediately preceded by a digit <= d
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9953 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether a number contains an even digit d immediately preceded by a digit <= d. */
static bool has_even_preceded(seq_t n)
{
    if (n < 10) return false;            /* one‑digit numbers never qualify */

    seq_t tmp = n;
    uint8_t digits[128];                  /* enough for up to 128‑bit values */
    size_t cnt = 0;

    while (tmp > 0) {
        digits[cnt++] = (uint8_t)(tmp % 10);  /* least significant digit first */
        tmp /= 10;
    }

    for (size_t i = 1; i < cnt; ++i) {
        uint8_t d = digits[i - 1];           /* current (right) digit */
        if ((d & 1) == 0) {                  /* d is even */
            if (digits[i] <= d)              /* preceding (left) digit <= d */
                return true;
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;            /* cached terms of the sequence */
    static size_t len = 0;                 /* number of cached terms */
    static size_t cap = 0;                 /* capacity of the cache */
    static seq_t next_candidate = 1;       /* next integer to test */

    /* Initialise cache on first call */
    if (cap == 0) {
        cap = 16;
        cache = mem_new(seq_t, cap);
        if (!cache) abort();
    }

    /* Generate terms until we have reached the desired index */
    while (len < (size_t)n) {
        if (has_even_preceded(next_candidate)) {
            if (len >= cap) {
                cap <<= 1;
                seq_t *tmp = mem_resize(seq_t, cache, cap);
                if (!tmp) abort();
                cache = tmp;
            }
            cache[len++] = next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}

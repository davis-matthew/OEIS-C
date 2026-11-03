/*
 * A253205  Natural numbers n such that n-1 is divisible by the sum of the
 *          decimal digits of n.
 * https://oeis.org/A253205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 2433, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9855 (2026-11-03)
 * Helpers:   digit_sum, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Natural numbers n such that n-1 is divisible by the sum of the decimal digits of n.
   The function generates members on demand and caches them for fast repeated access. */
#include <stddef.h>
#include <stdbool.h>
/* The following utilities are provided by the build environment. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

static seq_t *cache = NULL;         /* cached terms of the sequence */
static size_t cache_len = 0;        /* number of cached terms */
static size_t cache_cap = 0;        /* capacity of the cache array */
static seq_t last_checked = 0;      /* last integer that was examined */

/* Ensure that at least n terms are computed and cached. */
static void ensure_terms(idx_t n)
{
    while (cache_len < (size_t)n)
    {
        seq_t x = last_checked + 1;
        seq_t s = digit_sum(x);
        if (s != 0 && ((x - 1) % s) == 0)
        {
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = x;
        }
        last_checked = x;
    }
}

/* Return the nth member of the sequence (a(1) = 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1; n=0 is undefined but return 0 */
    ensure_terms(n);
    return cache[n - 1];
}

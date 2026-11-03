/*
 * A292455  Numbers where 5 outnumbers any other digit.
 * https://oeis.org/A292455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 4718, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 8731 (2026-11-03)
 * Helpers:   digit_counts, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursion: a(n) is the n-th positive integer whose count of digit 5
   strictly exceeds the count of any other digit. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/digit_counts.h"
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

static bool has_property(seq_t x)
{
    unsigned char cnt[10];
    digit_counts(x, cnt);
    unsigned char c5 = cnt[5];
    if (c5 == 0) {
        return false;                     /* need at least one 5 */
    }
    for (int d = 0; d < 10; ++d) {
        if (d != 5 && c5 <= cnt[d]) {
            return false;                 /* 5 must outnumber every other digit */
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *term_cache = NULL;   /* cached sequence values */
    static size_t cache_cap = 0;       /* allocated capacity */
    static size_t cache_len = 0;       /* number of terms stored */
    static seq_t next_candidate = 1;   /* next integer to test */

    if (n < 1) {
        return 0;                       /* undefined for n = 0, but safe */
    }

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        if (has_property(next_candidate)) {
            if (cache_len == cache_cap) {
                /* Grow the cache to accommodate one more term */
                grow_seq(&term_cache, &cache_cap, cache_len + 1);
            }
            term_cache[cache_len++] = next_candidate;
        }
        ++next_candidate;                /* next integer to test */
    }

    return term_cache[n - 1];
}

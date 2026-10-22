/*
 * A306643  Numbers that, for some x, are the concatenation of x, x+1 and x+2
 *          and are divisible by at least two of x, x+1 and x+2.
 * https://oeis.org/A306643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = the nth term of sequence A306643, i.e. the nth positive
 *            integer that is the concatenation of an integer x, x+1, x+2 and
 *            is divisible by at least two of those integers
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/pow10.h"

/* Memoized cache of computed terms */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static idx_t  last_x     = 0;

/* Return the number of decimal digits of a positive idx_t. */
static int num_digits(idx_t v) {
    int d = 0;
    while (v) {
        ++d;
        v /= 10;
    }
    return d ? d : 1;
}

/* Simple reallocation helper from the utils. */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size) {
    if (needed > *cap) {
        size_t new_cap = (*cap) ? (*cap) * 2 : 8;
        while (new_cap < needed) new_cap *= 2;
        *ptr = realloc(*ptr, new_cap * elem_size);
        *cap = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for offset 1, but harmless */

    while (cache_len < n) {        /* generate terms until we have n of them */
        ++last_x;                  /* candidate x */

        /* concatenate x, x+1, x+2 */
        seq_t N = (seq_t)last_x;
        int d1   = num_digits(last_x + 1);
        N *= pow10((idx_t)d1);
        N += (seq_t)(last_x + 1);

        int d2   = num_digits(last_x + 2);
        N *= pow10((idx_t)d2);
        N += (seq_t)(last_x + 2);

        /* count how many of x, x+1, x+2 divide N */
        int cnt = 0;
        if (N % (seq_t)last_x          == 0) ++cnt;
        if (N % (seq_t)(last_x + 1)    == 0) ++cnt;
        if (N % (seq_t)(last_x + 2)    == 0) ++cnt;

        if (cnt >= 2) {               /* store if condition satisfied */
            ensure_capacity(cache_len + 1, &cache_cap, (void**)&cache, sizeof(seq_t));
            cache[cache_len++] = N;
        }
    }
    return cache[n - 1];
}

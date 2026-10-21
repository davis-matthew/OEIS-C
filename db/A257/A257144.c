/*
 * A257144  Numbers n not of the form x+y*x^2 for x>1 and y>0.
 * https://oeis.org/A257144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer that cannot be written in the form x +
 *            y*x^2 with integer x > 1 and y > 0
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Test whether m is excluded (i.e. representable as x + y*x^2). */
static bool is_excluded(seq_t m)
{
    /* All numbers ≤ 5 cannot be written as x + y*x^2 (x > 1). */
    if (m <= 5)
        return false;

    for (idx_t x = 2;; ++x) {
        seq_t xsq = (seq_t)x * (seq_t)x;
        if (xsq > m)                /* once x^2 exceeds m we can stop */
            break;
        if (m < (seq_t)x + xsq)     /* need m ≥ x + x^2 to be possible */
            continue;
        seq_t diff = m - (seq_t)x;
        if ((diff % xsq) == 0) {
            seq_t y = diff / xsq;
            if (y >= 1)
                return true;
        }
    }
    return false;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;   /* number of terms stored */
    static idx_t last_checked = 1; /* next integer to examine */

    /* Ensure we have at least n terms cached. */
    if (cache_len < n) {
        /* Grow the cache array if necessary. */
        if (cache_cap < n) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 16;
            while (new_cap < n)
                new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute terms until we have n of them. */
        for (; cache_len < n; ++last_checked) {
            if (!is_excluded((seq_t)last_checked)) {
                cache[cache_len++] = last_checked;
            }
        }
    }

    return cache[n - 1];
}

/*
 * A291627  Numbers k such that 2 is the smallest decimal digit of k^2.
 * https://oeis.org/A291627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 40, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search algorithm for sequence A291627: numbers k such that 2 is the smallest
   decimal digit of k^2. */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Cache for discovered terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t found = 0;      /* Number of terms stored */
    static idx_t last_k = 1;      /* Next k to examine */

    /* Expand cache if necessary */
    if (found >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        seq_t *tmp_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp_cache;
        cache_cap = new_cap;
    }

    /* Generate terms until we have at least n */
    while (found < n) {
        seq_t k = (seq_t)last_k;
        seq_t k2 = k * k;          /* k^2 */

        /* Scan digits of k^2 */
        bool good = true;
        int min_digit = 9;
        seq_t tmp = k2;
        while (tmp > 0) {
            int d = (int)(tmp % 10);
            if (d < 2) {          /* contains 0 or 1 -> reject */
                good = false;
                break;
            }
            if (d < min_digit) min_digit = d;
            tmp /= 10;
        }

        if (good && min_digit == 2) {
            /* Store new term */
            cache[found++] = k;
        }
        ++last_k;
    }

    return cache[n - 1];
}

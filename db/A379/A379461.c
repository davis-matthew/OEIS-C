/*
 * A379461  Irregular triangle read by rows in which row n lists the divisors
 *          m of n such that there is a divisor d of n with d < m < 2*d, or 0
 *          if such divisors do not exist.
 * https://oeis.org/A379461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = nth element of an irregular triangle read by rows where
 *            each row n lists the divisors m of n that admit a divisor d of n
 *            with d < m < 2*d; the row is 0 if no such divisors exist
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* static state for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_row = 1;           /* next row to compute */

    if (n == 0) return 0; /* offset is 1; so n>=1 */

    /* extend cache until it contains at least n elements */
    while (cache_len < (size_t)n) {
        idx_t r = next_row;
        /* collect divisors of r */
        idx_t divs[256];
        size_t div_nb = 0;
        for (idx_t i = 1; i * i <= r; ++i) {
            if (r % i == 0) {
                divs[div_nb++] = i;
                if (i != r / i)
                    divs[div_nb++] = r / i;
            }
        }
        /* insertion sort the divisor array */
        for (size_t i = 1; i < div_nb; ++i) {
            idx_t key = divs[i];
            size_t j = i;
            while (j > 0 && divs[j - 1] > key) {
                divs[j] = divs[j - 1];
                --j;
            }
            divs[j] = key;
        }

        /* find qualifying divisors */
        seq_t row_vals[256];
        size_t row_len = 0;
        for (size_t i = 1; i < div_nb; ++i) {     /* skip m == 1 */
            idx_t m = divs[i];
            int ok = 0;
            for (size_t k = 0; k < div_nb && divs[k] < m; ++k) {
                seq_t two_d = (seq_t)divs[k] * 2;
                if ((seq_t)m < two_d) {
                    ok = 1;
                    break;
                }
            }
            if (ok) {
                row_vals[row_len++] = (seq_t)m;
            }
        }

        /* append to cache */
        if (row_len == 0) {
            size_t needed = cache_len + 1;
            if (needed > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                if (new_cap < needed) new_cap = needed;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) { free(cache); return 0; }
                cache = tmp; cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)0;
        } else {
            size_t needed = cache_len + row_len;
            if (needed > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 32;
                if (new_cap < needed) new_cap = needed;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) { free(cache); return 0; }
                cache = tmp; cache_cap = new_cap;
            }
            for (size_t i = 0; i < row_len; ++i)
                cache[cache_len++] = row_vals[i];
        }

        ++next_row;
    }

    return cache[(size_t)n - 1];
}

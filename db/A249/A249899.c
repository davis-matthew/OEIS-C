/*
 * A249899  Numbers k such that sigma(k) contains the same digits as k in base
 *          10.
 * https://oeis.org/A249899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 315, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_counts, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A249899: numbers k such that the set of decimal digits of sigma(k) equals the set of digits of k. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/digit_counts.h"

static seq_t *cache     = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static idx_t  last_checked = 0;

/* Make sure the cache can hold at least 'needed' elements. */
static void ensure_cache(size_t needed) {
    if (needed > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();  /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* offset 1: a(0) is not defined */

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        ++last_checked;                                 /* test next integer */

        seq_t sigma = sigma_of(last_checked);
        unsigned char cnt_k[10] = {0};
        unsigned char cnt_s[10] = {0};

        digit_counts((seq_t)last_checked, cnt_k);
        digit_counts(sigma, cnt_s);

        bool ok = true;
        for (int d = 0; d < 10; ++d) {
            if ((cnt_k[d] > 0) != (cnt_s[d] > 0)) {
                ok = false;
                break;
            }
        }

        if (ok) {
            ensure_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)last_checked;
        }
    }

    return cache[n - 1];
}

/*
 * A350533  Happy Niven (or happy harshad) numbers.
 * https://oeis.org/A350533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   terms are numbers divisible by the sum of their decimal digits
 *            (Harshad) and whose digit‑square trajectory reaches 1 (happy)
 * Range:     u8 n <= 9, u16 n <= 1436, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sum_square_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/sum_square_digits.h"

/* sum of decimal digits of x */
static seq_t digit_sum(seq_t x) {
    seq_t sum = 0;
    while (x) {
        sum += x % 10;
        x  /= 10;
    }
    return sum;
}

/* true if x is a Harshad number (divisible by its digit sum) */
static bool is_harshad(seq_t x) {
    seq_t s = digit_sum(x);
    return s != 0 && (x % s) == 0;
}

/* true if x is a happy number in base 10 */
static bool is_happy(seq_t x) {
    seq_t y = x;
    while (y != 1 && y != 4) {
        y = sum_square_digits(y);
    }
    return y == 1;
}

/* static cache of already found terms a(1)…a(cache_cnt) */
static seq_t *cache   = NULL;
static size_t cache_cnt = 0;
static size_t cache_cap = 0;

/* ensure the cache can hold at least 'need' elements */
static void grow_cache(size_t need) {
    if (need <= cache_cap) return;
    size_t newcap = cache_cap ? cache_cap * 2 : 16;
    while (newcap < need) newcap <<= 1;
    seq_t *tmp = (seq_t*)malloc(newcap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    if (cache) {
        memcpy(tmp, cache, cache_cnt * sizeof(seq_t));
        free(cache);
    }
    cache = tmp;
    cache_cap = newcap;
}

/* a(idx_t n) returns the nth term (n >= 1).  Calls may be in any order. */
seq_t a(idx_t n) {
    if (n < 1) return 0;                   /* sentinel for undefined n */
    if ((size_t)n <= cache_cnt)            /* already computed */
        return cache[(size_t)n - 1];

    /* start search from next integer after last cached term */
    seq_t cand = 1;
    if (cache_cnt > 0) cand = cache[cache_cnt - 1] + 1;

    while (1) {
        if (is_harshad(cand) && is_happy(cand)) {
            /* store new term */
            grow_cache(cache_cnt + 1);
            cache[cache_cnt++] = cand;
            if ((size_t)n <= cache_cnt)
                return cache[(size_t)n - 1];
        }
        ++cand;
    }
}

/*
 * A385116  Take the natural numbers, erase all occurrences of the digit "0,"
 *          and shift all remaining digits leftward without changing the
 *          position of commas.
 * https://oeis.org/A385116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   fill_nonzero_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A385116:
   For each n, a(n) is the integer formed by taking the next
   digit_count(n) digits from the stream obtained by concatenating
   the natural numbers with all '0' digits removed. */

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "../utils/fill_nonzero_digits.h"

/* helper: number of decimal digits of n (n >= 1) */
static inline int digit_count(uint64_t n) {
    int d = 1;
    while (n >= 10) {
        n /= 10;
        d++;
    }
    return d;
}

/* cache of computed terms */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_last = 0;   /* last cached index, 1‑based; 0 if none */

/* stream state for generating the digit stream */
static uint64_t stream_next_num = 1;      /* next natural number to use */
static uint8_t  stream_digits[20];          /* non‑zero digits of cur number */
static size_t   stream_digits_len = 0;      /* length of above array */
static size_t   stream_pos = 0;             /* next digit to consume */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* not used but safe */

    /* generate missing terms up to n if needed */
    if ((size_t)n > cache_last) {
        /* ensure enough capacity */
        if ((size_t)n > cache_cap) {
            size_t newcap = cache_cap ? cache_cap : 1;
            while (newcap < (size_t)n) newcap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            cache = tmp;
            cache_cap = newcap;
        }

        /* initialize stream on first use */
        if (stream_digits_len == 0) {
            stream_digits_len = fill_nonzero_digits(stream_next_num,
                                                    stream_digits);
            stream_pos = 0;
        }

        /* compute terms cache_last+1 .. n */
        for (idx_t i = cache_last + 1; i <= n; ++i) {
            int need = digit_count((uint64_t)i);
            seq_t val = 0;
            while (need > 0) {
                if (stream_pos >= stream_digits_len) {
                    stream_next_num++;
                    stream_digits_len = fill_nonzero_digits(stream_next_num,
                                                            stream_digits);
                    stream_pos = 0;
                }
                val = val * 10 + (seq_t)stream_digits[stream_pos++];
                need--;
            }
            cache[i - 1] = val;
        }
        cache_last = n;
    }
    return cache[n - 1];
}

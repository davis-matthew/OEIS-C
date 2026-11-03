/*
 * A283195  Clone sequence C(2,3): a(1) = 1; for n > 1, a(n) is obtained by
 *          repeating each digit of a(n-1), summing the digits in groups of 3,
 *          and concatenating the digits of the sums.
 * https://oeis.org/A283195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 1; for n > 1, a(n) is obtained by cloning each digit of
 *            a(n‑1), summing the digits in groups of 3, and concatenating the
 *            sums
 * Range:     u8 n <= 7, u16 n <= 20, u32 n <= 33, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Validated: partial, pass, 36 terms through n = 36 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t clone_sum(seq_t prev)
{
    /* Step 1: extract decimal digits of prev (most‑significant first). */
    size_t len = 0;
    uint8_t *digits = NULL;
    seq_t tmp = prev;

    if (tmp == 0) {
        len = 1;
        digits = (uint8_t*)malloc(len * sizeof(uint8_t));
        digits[0] = 0;
    } else {
        while (tmp > 0) {
            uint8_t d = (uint8_t)(tmp % 10);
            uint8_t *new_digits = (uint8_t*)realloc(digits, (len + 1) * sizeof(uint8_t));
            new_digits[len] = d;
            digits = new_digits;
            len++;
            tmp = tmp / 10;
        }
        /* Reverse to get most‑significant first */
        for (size_t i = 0; i < len / 2; ++i) {
            uint8_t t = digits[i];
            digits[i] = digits[len - 1 - i];
            digits[len - 1 - i] = t;
        }
    }

    /* Step 2: duplicate each digit. */
    size_t ext_len = 2 * len;
    size_t zeros_needed = (3 - (ext_len % 3)) % 3;
    size_t total = ext_len + zeros_needed;
    uint8_t *ext = (uint8_t*)malloc(total * sizeof(uint8_t));

    for (size_t i = 0; i < len; ++i) {
        ext[2 * i]     = digits[i];
        ext[2 * i + 1] = digits[i];
    }
    for (size_t i = 0; i < zeros_needed; ++i) {
        ext[ext_len + i] = 0;
    }

    /* Step 3: process groups of three, forming the result. */
    seq_t val = 0;
    for (size_t i = 0; i < total; i += 3) {
        uint8_t s = ext[i] + ext[i + 1] + ext[i + 2];
        if (s >= 10) {
            val = val * 10 + s / 10;
            val = val * 10 + s % 10;
        } else {
            val = val * 10 + s;
        }
    }

    free(digits);
    free(ext);
    return val;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* a(1) is the first term */
    if (n == 1) {
        return (seq_t)1;
    }

    /* Initialise cache if necessary */
    if (cache_len == 0) {
        cache = mem_new(seq_t, 1);
        cache[0] = (seq_t)1;      /* a(1) */
        cache_len = 1;
    }

    /* Expand cache up to n if required */
    if (n > cache_len) {
        cache = mem_resize(seq_t, cache, n);
        for (idx_t i = cache_len; i < n; ++i) {
            cache[i] = clone_sum(cache[i - 1]);
        }
        cache_len = n;
    }

    return cache[n - 1];
}

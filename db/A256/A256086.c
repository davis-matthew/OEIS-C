/*
 * A256086  Non-palindromic balanced numbers in base 6.
 * https://oeis.org/A256086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1734, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9858 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by exhaustive search of balanced base-6 non‑palindromic numbers */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Helper: test whether a number is a non‑palindromic balanced number in base 6 */
static bool is_balanced_nonpal(seq_t x)
{
    /* Convert x to base‑6 digits (most significant first) */
    uint8_t digits[512];
    size_t m = 0;

    if (x == 0) {
        digits[m++] = 0;
    } else {
        seq_t t = x;
        while (t > 0) {
            digits[m++] = (uint8_t)(t % 6);
            t /= 6;
        }
    }

    /* Reverse to most‑significant first */
    for (size_t i = 0, j = m - 1; i < j; ++i, --j) {
        uint8_t tmp = digits[i];
        digits[i] = digits[j];
        digits[j] = tmp;
    }

    /* Skip palindromes */
    bool ispal = true;
    for (size_t i = 0; i < m / 2; ++i) {
        if (digits[i] != digits[m - 1 - i]) {
            ispal = false;
            break;
        }
    }
    if (ispal) return false;

    /* Weighted sum test:  Σ d[i] * (2*i - (m-1)) == 0 */
    int64_t weighted = 0;
    int64_t m_minus1 = (int64_t)(m - 1);
    for (size_t i = 0; i < m; ++i) {
        int64_t w = 2 * (int64_t)i - m_minus1;
        weighted += (int64_t)digits[i] * w;
    }
    return weighted == 0;
}

seq_t a(idx_t n)
{
    static seq_t next_candidate = 1;
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;

    while (cache_size < n) {
        if (is_balanced_nonpal(next_candidate)) {
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();            /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = next_candidate;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}

/*
 * A252480  Numbers whose decimal representation has at least one '0' digit in
 *          a position other than the final digit.
 * https://oeis.org/A252480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity, get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose decimal representation has at least one '0'
   digit in a position other than the final digit.
   a(1) = 100, a(2) = 101, a(3) = 102, ... */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/get_digits.h"
#include "../utils/ensure_capacity.h"

static bool has_nonfinal_zero(seq_t x) {
    if (x < (seq_t)100) return false;
    size_t len;
    uint8_t digits[64];
    get_digits(x, digits, &len);
    /* Skip the least‑significant digit (index 0) */
    for (size_t i = 1; i < len; ++i) {
        if (digits[i] == 0) return true;
    }
    return false;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined offset behaviour not required */

    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    if ((size_t)n > cache_len) {
        seq_t candidate = cache_len ? cache[cache_len - 1] + 1 : (seq_t)99;
        while (cache_len < (size_t)n) {
            if (has_nonfinal_zero(candidate)) {
                ensure_capacity(cache_len + 1, &cache_cap, (void**)&cache, sizeof(seq_t));
                cache[cache_len++] = candidate;
            }
            candidate += 1;
        }
    }

    return cache[n - 1];
}

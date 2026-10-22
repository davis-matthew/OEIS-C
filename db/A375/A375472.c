/*
 * A375472  Least k such that the ternary representation of 2^k has exactly
 *          2*n 1's, or -1 if no such k exists.
 * https://oeis.org/A375472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A375472:  Least k such that the ternary representation of 2^k
 * has exactly 2*n 1's, or 0 if no such k exists.  (0 can be used as the
 * sentinel for “unknown”; actual values are always ≥1.)
 *
 * The function uses incremental computation with memoisation to handle
 * calls in any order.  The implementation keeps the base‑3 digits of the
 * current power of two as an array of uint8_t and updates the count of
 * 1‑digits in O(1) per digit during each multiplication by 2.  The search
 * proceeds until the requested index has been resolved.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>  /* size_t */

static void ensure_cache_cap(size_t needed, seq_t **cache, size_t *cap) {
    if (needed <= *cap) return;
    size_t new_cap = *cap ? *cap : 1;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(*cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);       /* allocation failure */
    for (size_t i = *cap; i < new_cap; ++i) tmp[i] = (seq_t)0;
    *cache = tmp;
    *cap   = new_cap;
}

static void ensure_digits_cap(size_t needed, uint8_t **digits, size_t *cap) {
    if (needed <= *cap) return;
    size_t new_cap = *cap ? *cap : 1;
    while (new_cap < needed) new_cap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(*digits, new_cap * sizeof(uint8_t));
    if (!tmp) exit(1);
    *digits = tmp;
    *cap    = new_cap;
}

seq_t a(idx_t n) {
    static bool initialized = false;
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t max_k_processed = 0;      /* exponent of the last 2^k processed */

    static uint8_t *digits = NULL;
    static size_t digit_cap = 0;
    static size_t digit_len = 0;
    static unsigned ones = 0;              /* current number of 1‑digits */

    /* initialise static state on first call */
    if (!initialized) {
        cache_cap = 1;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)1;               /* a(0) = 1 */
        max_k_processed = 0;               /* 2^0 currently stored */

        digit_cap = 1;
        digits = (uint8_t *)malloc(digit_cap * sizeof(uint8_t));
        if (!digits) exit(1);
        digits[0] = 1;                     /* 2^0 = 1 */
        digit_len = 1;
        ones = 1;                           /* one 1 in the representation */
        initialized = true;
    }

    /* be sure we have room for the requested index */
    ensure_cache_cap((size_t)n + 1, &cache, &cache_cap);

    /* answer immediately if already known */
    if (cache[(size_t)n] != (seq_t)0) return cache[(size_t)n];

    /* search until we find the required k */
    while (cache[(size_t)n] == (seq_t)0) {
        /* advance to the next power of two: multiply the current number by 2 */
        ++max_k_processed;

        /* grow digit array if necessary for the carry */
        size_t required_cap = digit_len + 1; /* worst case carry adds a digit */
        ensure_digits_cap(required_cap, &digits, &digit_cap);

        uint8_t carry = 0;
        for (size_t i = 0; i < digit_len; ++i) {
            uint8_t olddigit = digits[i];
            uint32_t temp    = (uint32_t)olddigit * 2u + carry;
            uint8_t newdigit = (uint8_t)(temp % 3u);
            carry = (uint8_t)(temp / 3u);

            digits[i] = newdigit;
            if (olddigit == 1) --ones;
            if (newdigit == 1) ++ones;
        }
        while (carry != 0) {
            uint8_t newdigit = carry % 3u;
            carry /= 3u;
            ensure_digits_cap(digit_len + 1, &digits, &digit_cap);
            digits[digit_len++] = newdigit;
            if (newdigit == 1) ++ones;
        }

        /* if the count of 1‑digits is even, we have found a candidate */
        if ((ones & 1u) == 0) {
            idx_t m = (idx_t)(ones >> 1);   /* target n value */
            ensure_cache_cap((size_t)m + 1, &cache, &cache_cap);
            if (cache[(size_t)m] == (seq_t)0) {
                cache[(size_t)m] = (seq_t)max_k_processed;
            }
        }
    }

    return cache[(size_t)n];
}

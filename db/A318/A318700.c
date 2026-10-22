/*
 * A318700  Positive numbers that contain odd and even digits.
 * https://oeis.org/A318700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search through positive integers that contain at least one odd and one even decimal digit */

#if defined(__cplusplus)
#include <vector>
#else
#include <stdlib.h>
#include <stdbool.h>
#endif
#include <stdint.h>

/* Helper to test the parity condition */
static bool has_odd_and_even(seq_t n)
{
    bool hasOdd = false, hasEven = false;
    while (n > 0) {
        unsigned digit = (unsigned)(n % 10);
        if (digit & 1) hasOdd = true; else hasEven = true;
        if (hasOdd && hasEven) return true;
        n /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for 0 but safe */

#if defined(__cplusplus)
    static std::vector<seq_t> cache;
    static seq_t last_checked = 9;
    size_t target = (size_t)n;
    if (cache.size() < target) {
        seq_t cand = last_checked;
        while (cache.size() < target) {
            cand++;
            if (has_odd_and_even(cand))
                cache.push_back(cand);
            last_checked = cand;
        }
    }
    return cache[(size_t)n - 1];
#else
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static seq_t last_checked = 9;

    size_t target = (size_t)n;
    if (cache == NULL) {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    while (cache_size < target) {
        seq_t cand = last_checked + 1;
        last_checked = cand;
        if (has_odd_and_even(cand)) {
            if (cache_size == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_size++] = cand;
        }
    }
    return cache[target - 1];
#endif
}

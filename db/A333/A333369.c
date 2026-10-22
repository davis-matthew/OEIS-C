/*
 * A333369  Positive integers in which any odd digit, if present, occurs an
 *          odd number of times, and any even digit, if present, occurs an
 *          even number of times.
 * https://oeis.org/A333369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 4003, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A333369 - n-th positive integer whose each odd digit occurs an odd number of
 * times and each even digit an even number of times. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_frequencies.h"

static bool is_simber(seq_t x)
{
    unsigned int freq[10] = {0};
    digit_frequencies(x, freq);
    for (int d = 0; d < 10; ++d) {
        if (freq[d] == 0) continue;
        if ((d & 1) == 0) {           /* even digit */
            if (freq[d] & 1) return false;
        } else {                       /* odd digit */
            if ((freq[d] & 1) == 0) return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap  = 0;
    static size_t len  = 0;
    static seq_t  next = 1;

    if (n == 0) return 0;           /* undefined for offset 0 */

    if (!cache) {
        cap = 32;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    while (len < (size_t)n) {
        if (is_simber(next)) {
            if (len >= cap) {
                cap <<= 1;
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[len++] = next;
        }
        ++next;
    }

    return cache[(size_t)n - 1];
}

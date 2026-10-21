/*
 * A296863  Numbers whose base-3 digits d(m), d(m-1), ..., d(0) have #(pits) <
 *          #(peaks); see Comments.
 * https://oeis.org/A296863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n-th natural number whose base‑3 digits have fewer pits
 *            than peaks
 * Range:     u8 n <= 36, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* return true iff base‑3 representation of num has #pits < #peaks */
static bool is_good(seq_t num) {
    unsigned char digits[128];
    int len = 0;
    seq_t temp = num;
    while (temp > 0) {
        digits[len++] = (unsigned char)(temp % 3);
        temp = temp / 3;
    }
    if (len < 3) return false;
    int pits = 0, peaks = 0;
    for (int i = 1; i + 1 < len; ++i) {
        unsigned char a = digits[i - 1];
        unsigned char b = digits[i];
        unsigned char c = digits[i + 1];
        if (a > b && b < c) pits++;
        if (a < b && b > c) peaks++;
    }
    return pits < peaks;
}

seq_t a(idx_t n) {
    static seq_t *cache   = NULL;
    static size_t cap     = 0;
    static size_t len     = 0;
    static seq_t next_candidate = 1;

    while (len < n) {
        if (is_good(next_candidate)) {
            if (len + 1 > cap) {
                size_t newcap = cap ? cap << 1 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cap = newcap;
            }
            cache[len++] = next_candidate;
        }
        next_candidate++;
    }
    return cache[(size_t)(n - 1)];
}

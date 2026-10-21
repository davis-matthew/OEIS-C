/*
 * A296888  Numbers whose base-12 digits d(m), d(m-1), ..., d(0) have #(pits)
 *          = #(peaks); see Comments.
 * https://oeis.org/A296888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth positive integer whose base‑12 digits satisfy *
 *            #(pits) = #(peaks) (a pit is a local minimum, a peak a local
 *            maximum)
 * Range:     u8 n <= 205, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

static bool equal_peaks_pits(seq_t x) {
    /* Convert x to base‑12 digits (least significant first). */
    int digits[128];
    int len = 0;
    seq_t t = x;
    if (t == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (t != 0) {
            digits[len++] = (int)(t % (seq_t)12);
            t /= (seq_t)12;
        }
    }
    int pit = 0, peak = 0;
    for (int i = 1; i + 1 < len; ++i) {
        int a = digits[i - 1];
        int b = digits[i];
        int c = digits[i + 1];
        if (a > b && b < c) pit++;
        if (a < b && b > c) peak++;
    }
    return pit == peak;
}

/* Cache the first N terms. 20000 is enough for n up to about 10000. */
#define CACHE_CAP 20000
static seq_t cache[CACHE_CAP];
static size_t cache_len = 0;
static seq_t last_val = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;                /* undefined, but harmless */
    while (cache_len < n) {
        seq_t curr = (cache_len == 0) ? (seq_t)1 : last_val + (seq_t)1;
        /* Find the next value with equal peaks and pits. */
        while (true) {
            if (equal_peaks_pits(curr)) {
                cache[cache_len++] = curr;
                last_val = curr;
                break;
            }
            curr = curr + (seq_t)1;
        }
    }
    return cache[n - 1];
}

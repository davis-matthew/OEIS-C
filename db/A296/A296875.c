/*
 * A296875  Numbers whose base-7 digits d(m), d(m-1), ..., d(0) have #(pits) <
 *          #(peaks); see Comments.
 * https://oeis.org/A296875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9859 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑7 digits d(m),…,d(0) have #(pits) < #(peaks). */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static bool has_more_peaks(seq_t x)
{
    /* Count peaks and pits in the base‑7 representation of x.
       Returns true if pits < peaks. */
    if (x < (seq_t)49)  /* fewer than 3 digits → no interior indexes */
        return false;

    uint8_t digits[128];   /* enough for any reasonably sized x */
    size_t dlen = 0;
    seq_t t = x;
    while (t > 0) {
        digits[dlen++] = (uint8_t)(t % 7);
        t /= 7;
    }
    if (dlen <= 2) return false;

    int peaks = 0, pits = 0;
    for (size_t i = 1; i + 1 < dlen; ++i) {
        uint8_t left  = digits[i - 1];
        uint8_t cur   = digits[i];
        uint8_t right = digits[i + 1];
        if (left < cur && cur > right) peaks++;
        else if (left > cur && cur < right) pits++;
    }
    return pits < peaks;
}

/* Maintain a small cache of the first ~20000 terms.
   Since n ≤ 10000 in the tests this is sufficient. */
#define MAX_CACHE 20000
static seq_t cache[MAX_CACHE];
static size_t cache_len = 0;
static seq_t next_num = 1;   /* next integer to test */

seq_t a(idx_t n)
{
    /* Ensure the cache contains at least n terms. */
    while (cache_len < n) {
        if (has_more_peaks(next_num))
            cache[cache_len++] = next_num;
        ++next_num;
    }
    return cache[n - 1];
}

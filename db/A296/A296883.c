/*
 * A296883  Numbers whose base-10 digits d(m), d(m-1), ..., d(0) have #(pits)
 *          > #(peaks); see Comments.
 * https://oeis.org/A296883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) obtained by searching for integers having more pits than
 *            peaks
 * Range:     u8 n <= 26, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 68 terms through n = 9030 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return true iff the decimal representation of x has more pits than peaks.
   The argument is taken as uint64_t so the function works regardless of the
   size of seq_t. */
static bool more_pits_than_peaks(uint64_t x)
{
    int digits[20]; /* sufficient for all uint64_t values (up to 20 decimal digits). */
    int len = 0;

    if (x == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (x > 0) {
            digits[len++] = (int)(x % 10);
            x /= 10;
        }
        /* Reverse to most‑significant order. */
        for (int i = 0; i < len / 2; ++i) {
            int t = digits[i];
            digits[i] = digits[len - 1 - i];
            digits[len - 1 - i] = t;
        }
    }

    if (len < 3)
        return false; /* need at least three digits to form a pit or peak */

    int pits = 0, peaks = 0;
    for (int i = 1; i <= len - 2; ++i) {
        int prev = digits[i - 1];
        int cur  = digits[i];
        int next = digits[i + 1];
        if (prev > cur && cur < next)
            ++pits;
        else if (prev < cur && cur > next)
            ++peaks;
    }
    return pits > peaks;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    idx_t found = 0;
    uint64_t cand = 1;

    while (1) {
        if (more_pits_than_peaks(cand)) {
            ++found;
            if (found == n)
                return (seq_t)cand;
        }
        ++cand;
    }
}

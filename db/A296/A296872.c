/*
 * A296872  Numbers whose base-6 digits d(m), d(m-1), ..., d(0) have #(pits) <
 *          #(peaks); see Comments.
 * https://oeis.org/A296872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9449 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n‑th integer whose base‑6 representation contains
   fewer pits than peaks. */
#include <stdbool.h>
#include <stddef.h>

/* Return true if the base‑6 digits of x have #pits < #peaks. */
static bool pits_less_peaks(seq_t x)
{
    unsigned char digits[64];
    size_t len = 0;
    seq_t y = x;
    while (y > 0) {
        digits[len++] = (unsigned char)(y % 6);
        y /= 6;
    }
    if (len < 3)
        return false;
    int pits = 0, peaks = 0;
    for (size_t i = 1; i + 1 < len; ++i) {
        unsigned char a = digits[i-1], b = digits[i], c = digits[i+1];
        if (a > b && b < c)
            pits++;
        else if (a < b && b > c)
            peaks++;
    }
    return pits < peaks;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t x = 36;              /* first 3‑digit base‑6 number */
    while (1) {
        if (pits_less_peaks(x)) {
            count++;
            if (count == n)
                return x;
        }
        ++x;
    }
}

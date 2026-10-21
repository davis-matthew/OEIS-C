/*
 * A296870  Numbers whose base-6 digits d(m), d(m-1), ..., d(0) have #(pits) =
 *          #(peaks); see Comments.
 * https://oeis.org/A296870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 142, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth natural number x such that the number of pits in the base‑6
   representation of x equals the number of peaks.  */
/* You may find the OEIS entry for more information. */
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* Return true if v has the same number of pits and peaks in its base‑6
   representation.  */
static bool equal_pits_peaks(seq_t v)
{
    /* Numbers with fewer than 3 base‑6 digits have no interior positions,
       hence 0 pits and 0 peaks, and thus are accepted.  */
    if (v < 7)
        return true;

    uint8_t digits[128];
    size_t len = 0;
    seq_t t = v;
    while (t > 0) {
        digits[len++] = (uint8_t)(t % 6);
        t /= 6;
    }

    size_t pits = 0, peaks = 0;
    for (size_t i = 1; i + 1 < len; ++i) {
        uint8_t a = digits[i - 1];
        uint8_t b = digits[i];
        uint8_t c = digits[i + 1];
        if (a > b && b < c)
            ++pits;
        else if (a < b && b > c)
            ++peaks;
    }
    return pits == peaks;
}

/* The function required by the specification.  */
seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t candidate = 1;
    while (true) {
        if (equal_pits_peaks(candidate)) {
            ++found;
            if (found == n)
                return candidate;
        }
        ++candidate;
    }
}

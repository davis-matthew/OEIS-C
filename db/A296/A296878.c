/*
 * A296878  Numbers whose base-8 digits d(m), d(m-1), ..., d(0) have #(pits) <
 *          #(peaks); see Comments.
 * https://oeis.org/A296878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n‑th positive integer whose base‑8 digits d(m)…d(0)
 *            satisfy * #(peaks) > #(pits), where a peak is d(i‑1) < d(i) >
 *            d(i+1) and a pit is * d(i‑1) > d(i) < d(i+1)
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static bool has_more_peaks(seq_t x)
{
    uint8_t digits[70];
    size_t len = 0;

    if (x == (seq_t)0) {
        digits[0] = 0;
        len = 1;
    } else {
        seq_t tmp = x;
        while (tmp != (seq_t)0) {
            uint8_t rem = (uint8_t)(tmp % (seq_t)8);
            digits[len++] = rem;
            tmp /= (seq_t)8;
        }
        /* reverse to most significant first */
        for (size_t i = 0; i < len / 2; ++i) {
            uint8_t t = digits[i];
            digits[i] = digits[len - 1 - i];
            digits[len - 1 - i] = t;
        }
    }

    if (len < 3)
        return false;

    int peaks = 0, pits = 0;
    for (size_t i = 1; i + 1 < len; ++i) {
        uint8_t left = digits[i - 1];
        uint8_t mid  = digits[i];
        uint8_t right= digits[i + 1];
        if (mid > left && mid > right)
            ++peaks;
        else if (mid < left && mid < right)
            ++pits;
    }
    return pits < peaks;
}

seq_t a(idx_t n)
{
    seq_t candidate = (seq_t)1;
    idx_t count = (idx_t)0;
    while (1) {
        if (has_more_peaks(candidate)) {
            ++count;
            if (count == n)
                return candidate;
        }
        ++candidate;
    }
}

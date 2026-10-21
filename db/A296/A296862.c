/*
 * A296862  Numbers whose base-3 digits d(m), d(m-1), ..., d(0) have #(pits) >
 *          #(peaks); see Comments.
 * https://oeis.org/A296862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 71, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: Sequence A296862 (Numbers with more pits than peaks in base‑3)
 * The function scans natural numbers in increasing order and returns the
 * n‑th one that satisfies the condition.
 */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Return true if the base‑3 representation of x has more pits than peaks. */
static bool has_more_pits_than_peaks(seq_t x)
{
    if (x < (seq_t)3) return false;          /* less than 2 base‑3 digits */

    /* Extract base‑3 digits: dig[0] = least significant digit */
    uint8_t dig[128];                       /* 128 digits are enough for 128‑bit numbers */
    size_t len = 0;
    seq_t tmp = x;
    while (tmp > (seq_t)0) {
        dig[len++] = (uint8_t)(tmp % (seq_t)3);
        tmp /= (seq_t)3;
    }
    if (len < 2) return false;              /* confirm at least 2 digits */

    uint32_t pits = 0, peaks = 0;
    for (size_t i = 1; i + 1 < len; ++i) {
        uint8_t a = dig[i - 1];
        uint8_t b = dig[i];
        uint8_t c = dig[i + 1];
        if (a > b && b < c) {
            ++pits;
        } else if (a < b && b > c) {
            ++peaks;
        }
    }
    return pits > peaks;
}

/* Return the n-th term of A296862.  The offset is 1. */
seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t current = (seq_t)1;

    while (1) {
        if (has_more_pits_than_peaks(current)) {
            ++count;
            if (count == n) return current;
        }
        ++current;
    }
}

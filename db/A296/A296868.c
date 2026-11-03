/*
 * A296868  Numbers whose base-5 digits d(m), d(m-1), ..., d(0) have #(pits) >
 *          #(peaks); see Comments.
 * https://oeis.org/A296868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth natural number whose base‑5 digits contain
 *            strictly more pits than peaks (found by exhaustive search)
 * Range:     u8 n <= 44, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9765 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Return true iff the base‑5 representation of x has more pits
   than peaks.  */
static bool is_pits_gt_peaks(seq_t x)
{
    /* Extract base‑5 digits into a small array.  */
    unsigned char digits[64];   /* 5^64 > 10^44 > 2^128, enough for 128‑bit types */
    size_t len = 0;
    while (x != (seq_t)0) {
        digits[len++] = (unsigned char)(x % (seq_t)5);
        x = x / (seq_t)5;
    }
    if (len < 3)
        return false;          /* cannot have a pit or peak */

    int pits = 0, peaks = 0;
    for (size_t i = 1; i + 1 < len; ++i) {
        unsigned char d_prev = digits[i - 1];
        unsigned char d_curr = digits[i];
        unsigned char d_next = digits[i + 1];
        if (d_prev > d_curr && d_curr < d_next)
            ++pits;
        else if (d_prev < d_curr && d_curr > d_next)
            ++peaks;
    }
    return pits > peaks;
}

/* Return the nth element (1‑based) of the sequence A296868.  */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined, but avoid underflow when n=1 */
        return (seq_t)0;

    seq_t val = (seq_t)1;    /* start from 1 */
    idx_t count = 0;         /* how many qualifying numbers found */
    while (1) {
        if (is_pits_gt_peaks(val)) {
            ++count;
            if (count == n)
                return val;
        }
        ++val;
    }
}

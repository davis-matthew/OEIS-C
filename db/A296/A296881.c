/*
 * A296881  Numbers whose base-9 digits d(m), d(m-1), ..., d(0) have #(pits) <
 *          #(peaks); see Comments.
 * https://oeis.org/A296881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A296881: a(n) = the nth natural number whose base‑9
   digits d(m)…d(0) satisfy #(pits) < #(peaks).  The function
   scans forward from 0 until it finds the nth qualifying number. */
#include <stdbool.h>

static bool is_valid(seq_t x)
{
    /* base‑9 digits are stored least‑significant first */
    uint8_t d[128];
    int len = 0;

    seq_t t = x;
    while (t != 0) {
        d[len++] = (uint8_t)(t % 9);
        t /= 9;
        /* assume len will not exceed 128 for all values that
           appear in the sequence up to the test limits */
    }

    if (len < 3) {
        /* need a middle digit to form a pit or a peak */
        return false;
    }

    int pits = 0, peaks = 0;
    for (int i = 1; i + 1 < len; ++i) {
        if (d[i-1] > d[i] && d[i] < d[i+1]) {
            ++pits;
        } else if (d[i-1] < d[i] && d[i] > d[i+1]) {
            ++peaks;
        }
    }
    return pits < peaks;
}

seq_t a(idx_t n)
{
    seq_t cur = 0;
    idx_t count = 0;

    while (true) {
        if (is_valid(cur)) {
            ++count;
            if (count == n) return cur;
        }
        ++cur;
    }
}

/*
 * A296861  Numbers whose base-3 digits d(m), d(m-1), ..., d(0) have #(pits) =
 *          #(peaks); see Comments.
 * https://oeis.org/A296861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 148, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9045 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n‑th natural number whose base‑3 representation has an
 * equal number of pits and peaks.  The function searches sequentially
 * from 1 upwards and returns the n‑th match.  This satisfies both the
 * C11 and C++17 build requirements without relying on dynamic memory
 * allocation. */
#include <stdbool.h>
#include <stddef.h>

static bool equal_pits_peaks(seq_t x)
{
    /* For digits < 3 there are no interior positions → equal. */
    if (x < (seq_t)3) return true;

    /* Temporary buffer for the base‑3 digits (least‑significant first). */
    uint8_t buf[128];          /* enough for 128‑bit values (≤86 digits) */
    size_t len = 0;
    seq_t t = x;
    while (t > (seq_t)0) {
        buf[len++] = (uint8_t)(t % (seq_t)3);
        t /= (seq_t)3;
    }
    if (len < 3) return true;

    int pits = 0, peaks = 0;
    for (size_t i = 1; i + 1 < len; ++i) {
        uint8_t a = buf[i - 1], b = buf[i], c = buf[i + 1];
        if (a > b && b < c) ++pits;
        else if (a < b && b > c) ++peaks;
    }
    return pits == peaks;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* defined only for n ≥ 1, but handle 0 gracefully */
    idx_t cnt = 0;
    idx_t candidate = 0;
    while (cnt < n) {
        ++candidate;
        if (equal_pits_peaks((seq_t)candidate)) ++cnt;
    }
    return (seq_t)candidate;
}

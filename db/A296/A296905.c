/*
 * A296905  Numbers whose base-20 digits d(m), d(m-1), ..., d(0) have #(pits)
 *          < #(peaks); see Comments.
 * https://oeis.org/A296905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n‑th natural number whose base‑20 * representation
 *            has fewer pits than peaks. * A pit is an index i such that
 *            d(i‑1) > d(i) < d(i+1); * a peak is an index i such that d(i‑1)
 *            < d(i) > d(i+1). * The function scans successive integers until
 *            the n‑th qualifying * value is found. It works f...
 * Range:     u8 none, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/*  Return true if the base‑20 representation of x has more peaks than
 *  pits.  Numbers with fewer than three base‑20 digits cannot satisfy
 *  the condition, so the function returns false for them.  */
static bool has_more_peaks(seq_t x)
{
    /* 256 digits are enough for all numbers we will test (the first
     * 24 terms are below 2000, and n ≤ 10000 leads to candidates < 1e5).  */
    uint8_t buf[256];
    size_t len = 0;
    seq_t y = x;

    if (y == 0) {
        buf[len++] = 0;            /* base‑20 representation of 0 */
    } else {
        while (y > 0) {
            unsigned digit = (unsigned)(y % 20);  /* digit 0 … 19 */
            buf[len++] = (uint8_t)digit;
            y = y / 20;
        }
    }

    if (len < 3)
        return false;                /* cannot have pits or peaks */

    size_t pits = 0, peaks = 0;
    for (size_t i = 1; i + 1 < len; ++i) {
        unsigned a = buf[i - 1];
        unsigned b = buf[i];
        unsigned c = buf[i + 1];

        if (a > b && b < c)
            ++pits;
        else if (a < b && b > c)
            ++peaks;
    }
    return peaks > pits;
}

/*  Main function of the sequence.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined for n = 0, but avoid UB */

    seq_t candidate = 400;           /* first number with ≥3 base‑20 digits */
    idx_t found = 0;

    while (found < n) {
        if (has_more_peaks(candidate)) {
            ++found;
            if (found == n)
                return candidate;
        }
        ++candidate;                 /* next natural number */
    }

    return 0;                        /* should never reach here */
}

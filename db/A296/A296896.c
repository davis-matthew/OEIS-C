/*
 * A296896  Numbers whose base-14 digits d(m), d(m-1), ..., d(0) have #(pits)
 *          < #(peaks); see Comments.
 * https://oeis.org/A296896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth natural number whose base-14 digits contain
 *            strictly more peaks than pits. The function performs a linear
 *            search over positive integers, testing each candidate by
 *            counting local peaks and pits in its base‑14 representation
 * Range:     u8 n <= 9, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

static inline bool good_number(seq_t x) {
    /* Extract base‑14 digits into an array.
       We use a sufficiently large fixed buffer; values
       encountered for the first 10 000 terms are far
       smaller than the limit. */
    unsigned char digits[2048];
    int len = 0;
    seq_t tmp = x;

    while (tmp > 0) {
        digits[len++] = (unsigned char)(tmp % 14);
        tmp /= 14;
    }
    if (len < 3)  /* cannot have more peaks than pits */
        return false;

    int pits = 0, peaks = 0;
    for (int i = 1; i + 1 < len; ++i) {
        unsigned char dm = digits[i - 1];
        unsigned char d0 = digits[i];
        unsigned char dp = digits[i + 1];

        if (dm > d0 && d0 < dp)
            ++pits;
        else if (dm < d0 && d0 > dp)
            ++peaks;
    }
    return peaks > pits;
}

seq_t a(idx_t n) {
    if (n == 0)  /* not defined for offset 0, but guard anyway */
        return 0;

    seq_t val = 0;
    idx_t count = 0;

    for (;;) {
        if (good_number(val)) {
            ++count;
            if (count == n)
                return val;
        }
        ++val;   /* advance to next candidate */
    }
}

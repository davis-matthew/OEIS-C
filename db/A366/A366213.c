/*
 * A366213  Numbers whose median of the digits is equal to 7.
 * https://oeis.org/A366213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   digit_counts
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This routine generates the n‑th term of OEIS A366213 by brute‑force
   scanning natural numbers and testing whether the median of their
   decimal digits (after sorting the digits) equals 7. The median is
   computed as the middle digit for odd lengths, or as the average of
   the two middle digits for even lengths. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_counts.h"

static bool median_is7(idx_t x)
{
    unsigned char cnt[10];
    digit_counts((seq_t)x, cnt);

    idx_t len = 0;
    for (int d = 0; d < 10; ++d) len += cnt[d];

    if (len == 0) return false;          /* should not happen */

    if (len & 1) {                        /* odd length */
        idx_t k = (len + 1) / 2;          /* 1‑based position of median */
        idx_t cum = 0;
        for (int d = 0; d < 10; ++d) {
            cum += cnt[d];
            if (cum >= k) return d == 7;
        }
    } else {                              /* even length */
        idx_t k1 = len / 2;               /* first middle position (1‑based) */
        idx_t k2 = k1 + 1;                /* second middle position */
        idx_t cum = 0;
        int first = -1, second = -1;
        for (int d = 0; d < 10; ++d) {
            cum += cnt[d];
            if (cum >= k1 && first == -1) first = d;
            if (cum >= k2) { second = d; break; }
        }
        return first != -1 && second != -1 && (first + second) == 14;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t num = 1;

    for (;; ++num) {
        if (median_is7(num)) {
            ++count;
            if (count == n) return (seq_t)num;
        }
    }
}

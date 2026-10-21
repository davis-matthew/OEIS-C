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
 * Helpers:   pits_less_peaks
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n‑th integer whose base‑6 representation contains
   fewer pits than peaks. */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/pits_less_peaks.h"

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

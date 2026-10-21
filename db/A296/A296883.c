/*
 * A296883  Numbers whose base-10 digits d(m), d(m-1), ..., d(0) have #(pits)
 *          > #(peaks); see Comments.
 * https://oeis.org/A296883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) obtained by searching for integers having more pits than
 *            peaks
 * Range:     u8 n <= 26, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   more_pits_than_peaks
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/more_pits_than_peaks.h"

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    idx_t found = 0;
    uint64_t cand = 1;

    while (1) {
        if (more_pits_than_peaks(cand)) {
            ++found;
            if (found == n)
                return (seq_t)cand;
        }
        ++cand;
    }
}

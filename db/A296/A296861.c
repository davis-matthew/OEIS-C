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
 * Helpers:   equal_pits_peaks
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
#include "../utils/equal_pits_peaks.h"

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

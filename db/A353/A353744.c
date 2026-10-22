/*
 * A353744  Numbers k such that the k-th composition in standard order has all
 *          equal run-lengths.
 * https://oeis.org/A353744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   equal_run_lengths
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A353744: a(n) is the n-th nonnegative integer k such that the
   k-th composition in standard order has all equal run‑lengths.
   Implementation enumerates k progressively. */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/equal_run_lengths.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, but safe */

    idx_t count = 0;
    idx_t k = 0;

    while (1) {
        if (equal_run_lengths(k)) {
            ++count;
            if (count == n) return (seq_t)k;
        }
        ++k;
    }
}

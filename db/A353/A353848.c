/*
 * A353848  Numbers k such that the k-th composition in standard order (row k
 *          of A066099) has all equal run-sums.
 * https://oeis.org/A353848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   has_equal_run_sums
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A353848: numbers k such that the k‑th composition in standard
   order (graded reverse‑lexicographic) has all equal run‑sums.
   The code enumerates compositions and returns the n‑th valid k. */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/has_equal_run_sums.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 0; ; ++k)
    {
        if (has_equal_run_sums(k))
        {
            if (count == n)
                return (seq_t)k;
            ++count;
        }
    }
}

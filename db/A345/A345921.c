/*
 * A345921  Numbers k such that the k-th composition in standard order (row k
 *          of A066099) has alternating sum != 0.
 * https://oeis.org/A345921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer k for which the k‑th composition in *
 *            standard order (A066099) has non‑zero alternating sum. * This
 *            implementation scans k incrementally and counts the qualifying *
 *            values
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   has_nonzero_alt_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_nonzero_alt_sum.h"

/* Return the n‑th term of A345921 (1‑based). */
seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 0;
    while (count < n) {
        ++k;
        if (has_nonzero_alt_sum(k))
            ++count;
    }
    return (seq_t)k;
}

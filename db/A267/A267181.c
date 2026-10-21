/*
 * A267181  Array read by antidiagonals: T(i,j) (i>=0, j>=0) = number of steps
 *          to reach either top row or main diagonal using the steps
 *          (i,j)->(j,i) or (i,j)->(i,j-i).
 * https://oeis.org/A267181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the value of T(i,j) read along antidiagonals, where
 *            T(i,j) is defined by T(0,k) = T(k,k) = 0 T(i,j) = T(j,i) + 1 if
 *            i > j T(i,j) = T(i,j-i) + 1 if j > i The helper below evaluates
 *            this recurrence efficiently by an accelerated subtraction
 *            Euclid‑like algorithm
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   steps_for_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/steps_for_pair.h"

seq_t a(idx_t n)
{
    /* Find diagonal d so that n lies in the range  
       [sum_{k=0}^{d-1}(k+1), sum_{k=0}^{d}(k+1)) . */
    idx_t d = 0;
    idx_t start = 0;                   /* first index on current diagonal */
    while (1) {
        idx_t len = d + 1;
        if (n < start + len) break;
        start += len;
        d++;
    }

    idx_t offset = n - start;          /* position within diagonal, 0‑based */
    seq_t i = (seq_t)(d - offset);
    seq_t j = (seq_t)offset;
    return steps_for_pair(i, j);
}

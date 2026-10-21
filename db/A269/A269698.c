/*
 * A269698  First differences of number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 6", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A269698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 13, i16 n <= 253, i32 n <= 299, i64 n <= 299,
 *            i128 n <= 299, big any n
 * Helpers:   cell_count, pow4_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A269698
    a(n) = c(n+1) - c(n)
    where c(n) = 4^{popcount(n)}              if n is even
          c(n) = 5 * 4^{popcount(n)-1}        if n is odd
    (popcount is the number of 1-bits in n)
*/

#include <stdint.h>
#include "../utils/cell_count.h"
#include "../utils/pow4_u64.h"

/* main sequence function */
seq_t a(idx_t n)
{
    return cell_count(n + 1) - cell_count(n);
}

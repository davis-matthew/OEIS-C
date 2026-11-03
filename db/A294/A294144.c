/*
 * A294144  Sum of the smaller parts of the partitions of 2n into two parts
 *          with larger part squarefree.
 * https://oeis.org/A294144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} i * is_squarefree(2*n-i)
 * Range:     u8 n <= 27, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Validated: partial, pass, 56 terms through n = 56 (2026-11-01)
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    seq_t tot = 0;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t x = (seq_t)n * (seq_t)2;
        x -= (seq_t)i;
        if (is_squarefree(x))
            tot += (seq_t)i;
    }
    return tot;
}

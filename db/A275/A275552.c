/*
 * A275552  Number of classes of endofunctions of [n] under vertical
 *          translation mod n and complement to n+1.
 * https://oeis.org/A275552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^(n-1)+c)/2, where c = 1 if n is odd c = 2^(n-1) if n
 *            is even a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 100 (2026-11-01)
 * Helpers:   ipow_idx, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* compute n^(n-1) */
    seq_t nn = ipow_idx((seq_t)n, n - 1);

    /* choose the constant part c */
    seq_t c;
    if (n & 1)  /* odd */
        c = (seq_t)1;
    else        /* even */
        c = pow2(n - 1);

    /* (nn + c) / 2 */
    return (nn + c) / (seq_t)2;
}

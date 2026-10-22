/*
 * A391717  a(n) = (n^(n-1) + n^(n+1) - (n+1)^(n-1) - (n-1)^(n+1))/2.
 * https://oeis.org/A391717
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^(n‑1) + n^(n+1) – (n+1)^(n‑1) – (n‑1)^(n+1))/2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* For n below the official offset we return 0. */
    if (n < 2)
        return (seq_t)0;

    /* Compute the four required powers. */
    seq_t p1 = pow_uint((uint64_t)n, n - 1);      /* n^(n-1)   */
    seq_t p2 = pow_uint((uint64_t)n, n + 1);      /* n^(n+1)   */
    seq_t p3 = pow_uint((uint64_t)(n + 1), n - 1);/* (n+1)^(n-1) */
    seq_t p4 = pow_uint((uint64_t)(n - 1), n + 1);/* (n-1)^(n+1) */

    /* The expression is (p1 + p2) - (p3 + p4) and then divided by 2. */
    seq_t sum   = p1 + p2;
    seq_t sub   = p3 + p4;
    seq_t diff  = sum - sub;           /* diff is non‑negative for n ≥ 2 */
    return diff / (seq_t)2;
}

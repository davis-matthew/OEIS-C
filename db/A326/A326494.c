/*
 * A326494  Number of subsets of {1..n} containing all differences and
 *          quotients of pairs of distinct elements.
 * https://oeis.org/A326494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1, 2*n (1≤n≤3), 2*n+1 (n≥4)
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>  /* only for bool if needed */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n <= 3)
        return (seq_t)2 * (seq_t)n;
    return (seq_t)2 * (seq_t)n + (seq_t)1;
}

/*
 * A288834  a(n) = (n+1) * 3^(n-1).
 * https://oeis.org/A288834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1) * 3^(n-1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* For safety, handle n == 0 (outside of OEIS offset) */
    if (n == 0) return (seq_t)0;
    /* Compute 3^(n-1) using the helper and multiply by (n+1). */
    return (seq_t)(n + 1) * pow3(n - 1);
}

/*
 * A356874  Write n as Sum_{i in S} 2^(i-1), where S is a set of positive
 *          integers, then a(n) = Sum_{i in S} F_i, where F_i is the i-th
 *          Fibonacci number, A000045(i).
 * https://oeis.org/A356874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum_{i in S} F_i, where S are the positions of ones in
 *            the binary expansion of n (position 1 corresponds to 2^0).
 *            Equivalently, a(n) = Σ_{j≥0, (n>>j)&1} F_{j+1}
 * Range:     u8 n <= 2906, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 0)              /* a(0) = 0 */
        return 0;

    seq_t sum = 0;
    idx_t pos = 1;           /* Fibonacci index (1‑based) */
    idx_t tmp   = n;

    while (tmp) {
        if (tmp & 1U)             /* bit j is set → add F_{pos} */
            sum += fib(pos);
        tmp >>= 1U;
        pos  += 1U;
    }
    return sum;
}

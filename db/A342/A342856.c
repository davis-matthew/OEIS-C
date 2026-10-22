/*
 * A342856  Factorial numbers n that are sqrt(n)-smooth.
 * https://oeis.org/A342856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n ≥ 2, a(n) = (n+2)! (factorial of n+2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return factorial((idx_t)1);
    return factorial((idx_t)(n + (idx_t)2));
}

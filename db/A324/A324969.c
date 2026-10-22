/*
 * A324969  Number of unlabeled rooted identity trees with n vertices whose
 *          non-leaf terminal subtrees are all different.
 * https://oeis.org/A324969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n-1) for n ≥ 2; a(1) = 1
 * Range:     u8 n <= 14, u16 n <= 25, u32 n <= 48, u64 n <= 94, u128 n <= 187,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;
    return fib(n-1);
}

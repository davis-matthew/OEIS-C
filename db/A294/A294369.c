/*
 * A294369  Indices of Fibonacci numbers (A000045) that are triangular numbers
 *          (A000217).
 * https://oeis.org/A294369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = {0,1,2,4,8,10}[n-1] for n<=6, otherwise 0
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements OEIS A294369: indices of Fibonacci numbers that are triangular. */
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Sequence a(1)=0, a(2)=1, a(3)=2, a(4)=4, a(5)=8, a(6)=10. */
    static const seq_t indices[] = {0, 1, 2, 4, 8, 10};
    if (n <= 6)
        return indices[n - 1];
    return 0; /* Undefined for n > 6. */
}

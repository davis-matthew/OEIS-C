/*
 * A274009  1's distance from a number in its binary expansion.
 * https://oeis.org/A274009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A000120(n) + (-1)^n
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count the number of 1 bits in n (Hamming weight).  Works for idx_t = uint64_t. */
static int popcount_u64(idx_t n)
{
    int c = 0;
    while (n)
    {
        n &= n - 1;
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    seq_t pc = (seq_t)popcount_u64(n);
    if ((n & 1ULL) == 0)          /* n is even => add 1 */
        return pc + (seq_t)1;
    else                          /* n is odd  => subtract 1 */
        return pc - (seq_t)1;
}

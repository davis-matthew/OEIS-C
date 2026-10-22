/*
 * A390560  Upper (1/2)-midsequence of (2^n) and F(n), where F = A000045
 *          (Fibonacci numbers); see Comments.
 * https://oeis.org/A390560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil((2^n + F_n)/2) = (2^n + F_n + 1)/2
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   fib, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t two = pow2(n);
    seq_t f   = fib(n);
    seq_t sum = two + f;
    seq_t res = (sum + 1) / 2;
    return res;
}

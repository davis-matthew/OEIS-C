/*
 * A259221  Number of (n+1) X (7+1) 0..1 arrays with each 2 X 2 subblock
 *          having clockwise pattern 0000 0011 or 0101.
 * https://oeis.org/A259221
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n+1) + 53*Fibonacci(n+3) + 148
 * Range:     u8 none, u16 n <= 12, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   fib, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset 1, but safe guard */
    seq_t term2 = pow2(n + 1);         /* 2^(n+1) */
    seq_t fq = fib(n + 3);             /* Fibonacci(n+3) */
    seq_t term3 = 53 * fq;             /* 53 * Fibonacci(n+3) */
    return term2 + term3 + 148;        /* final value */
}

/*
 * A368877  a(n) = f^k(n) where f(n) = A014682(n), the Collatz map, and k =
 *          A070939(n), the length of n in base 2.
 * https://oeis.org/A368877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (f^k)(n) where * f(m) = m/2 if m is even, * (3*m+1)/2 if
 *            m is odd, * k = floor(log2(n)) + 1 (bitlength of n) * * Example:
 *            a(5) = f(f(f(5))) = f(f(8)) = f(4) = 2
 * Range:     u8 n <= 62, u16 n <= 2046, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* n>=1 in the sequence, but guard anyway */
    // Compute length of n in binary (k >= 1)
    // n is unsigned 64-bit per problem description
    uint64_t k = 64 - __builtin_clzll((uint64_t)n);
    seq_t x = (seq_t)n;

    for (uint64_t i = 0; i < k; ++i) {
        if ((x & 1) == 0) {            // even
            x >>= 1;                   // x = x/2
        } else {                       // odd
            x = (x * 3 + 1) >> 1;      // x = (3*x + 1)/2
        }
    }
    return x;
}

/*
 * A257808  a(n) = number of nonzero even numbers in range 0 .. n of A233271,
 *          the infinite trunk of inverted binary beanstalk.
 * https://oeis.org/A257808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; for n >= 1, a(n) = a(n-1) + (1 if A233271(n) is even
 *            and nonzero, else 0), where A233271(0) = 0, A233271(k+1) =
 *            A233271(k) + 1 + (number of 0's in binary of A233271(k))
 * Range:     u8 n <= 73, u16 n <= 8911, u32 n <= 16405, u64 n <= 16405,
 *            u128 n <= 16405, big any n
 * Validated: standard, pass, 105 terms through n = 9718 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t x = 0;
    seq_t count = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t t = x;
        seq_t zeros = 0;
        while (t != 0)
        {
            if ((t & 1) == 0)
                zeros++;
            t >>= 1;
        }
        x = x + 1 + zeros;
        if ((x & 1) == 0)
            count++;
    }
    return count;
}

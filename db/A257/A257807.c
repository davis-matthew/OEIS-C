/*
 * A257807  a(n) = number of odd numbers in range 0 .. n of A233271, the
 *          infinite trunk of inverted binary beanstalk.
 * https://oeis.org/A257807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of odd terms among t(0..n), where t(0)=0, t(k+1) =
 *            t(k) + 1 + (number of 0's in binary expansion of t(k)) (A233271)
 * Range:     u8 n <= 73, u16 n <= 9481, u32 n <= 16405, u64 n <= 16405,
 *            u128 n <= 16405, big any n
 * Validated: standard, pass, 107 terms through n = 9482 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    seq_t t = 0;
    idx_t count = 0;
    for (idx_t i = 0; ; ++i)
    {
        if ((t & 1) == 1)
            count++;
        if (i == n)
            break;
        unsigned zeros = 0;
        seq_t u = t;
        while (u != 0)
        {
            if ((u & 1) == 0)
                zeros++;
            u >>= 1;
        }
        t = t + 1 + zeros;
    }
    return (seq_t)count;
}

/*
 * A333291  a(n) = Sum_{i=1..n, gcd(i,n)=1} i*phi(i) where phi is Euler's
 *          totient function A000010.
 * https://oeis.org/A333291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = Σ_{i=1..n, gcd(i,n)=1} i*phi(i)
 * Range:     u8 n <= 12, u16 n <= 70, u32 n <= 2776, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9255 (2026-11-01)
 * Helpers:   gcd, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/totient.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        if (gcd((seq_t)i, (seq_t)n) == 1)
            sum += (seq_t)i * totient(i);
    }
    return sum;
}

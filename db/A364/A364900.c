/*
 * A364900  The n-volume of the unit regular n-simplex is
 *          sqrt(a(n))/A364901(n), with a(n) being squarefree.
 * https://oeis.org/A364900
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = squarefree_part( (n+1) if n even, (n+1)/2 if n odd )
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9965 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return the squarefree part of x: product of primes with odd exponent in x */
static seq_t squarefree_part(idx_t x)
{
    seq_t res = 1;
    idx_t p = 2;
    while (p * p <= x) {
        if (x % p == 0) {
            bool odd = false;
            while (x % p == 0) {
                odd = !odd;
                x /= p;
            }
            if (odd)
                res *= (seq_t)p;
        }
        if (p == 2)
            p = 3;
        else
            ++p;
    }
    if (x > 1)   /* remaining prime factor with exponent 1 (odd) */
        res *= (seq_t)x;
    return res;
}

seq_t a(idx_t n)
{
    idx_t val;
    if ((n & 1U) == 0)      /* even n */
        val = n + 1;
    else                   /* odd n */
        val = (n + 1) / 2;
    return squarefree_part(val);
}

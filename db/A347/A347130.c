/*
 * A347130  a(n) = Sum_{d|n} d * A003415(n/d), where A003415 is the arithmetic
 *          derivative.
 * https://oeis.org/A347130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d * A003415(n/d)
 * Range:     u8 n <= 35, u16 n <= 1439, u32 n <= 9888, u64 n <= 9888,
 *            u128 n <= 9888, big any n
 * Helpers:   _divisor_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dirichlet convolution of the identity function with the arithmetic derivative. */

#include <stddef.h>
#include <stdint.h>
#include "../utils/_divisor_rec.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, but safe */

    uint64_t N = (uint64_t)n;

    /* Factor n into small primes (trial division). */
    uint64_t primes[20];
    int exps[20];
    size_t pcnt = 0;

    uint64_t temp = N;
    for (uint64_t p = 2; p * p <= temp; p += (p == 2 ? 1 : 2)) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt]   = e;
            ++pcnt;
        }
    }
    if (temp > 1) {
        primes[pcnt] = temp;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    int di[20] = {0};
    seq_t sum  = 0;

    _divisor_rec(0, (seq_t)1, di, &sum, N, primes, exps, pcnt);

    return sum;
}

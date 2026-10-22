/*
 * A374051  a(n) = 1 if A113177(n) is a multiple of 3, otherwise 0, where
 *          A113177 is fully additive with a(p) = Fibonacci(p).
 * https://oeis.org/A374051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if A113177(n) is a multiple of 3, otherwise 0
 * Range:     u8 n <= 9582, u16 n <= 9582, u32 n <= 9582, u64 n <= 9582,
 *            u128 n <= 9582, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A113177 is fully additive with a(p) = Fibonacci(p).  Modulo 3 this
   reduces to the Fibonacci sequence modulo 3, which has period 8:
   fib_mod3 = {0,1,1,2,0,2,2,1}.  We factor n, accumulate the sum of
   e * fib_mod3[p % 8] for each prime factor p^e of n, and return
   1 if that sum ≡ 0 (mod 3). */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

/* Fibonacci modulo 3, period 8 */
static const unsigned char fib_mod3[8] = {0, 1, 1, 2, 0, 2, 2, 1};

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;      /* undefined for n < 1, return 0 */

    /* Factor n */
    uint64_t pa[64], ex[64];
    size_t cnt;
    factor_u64((uint64_t)n, pa, ex, &cnt);

    unsigned sum_mod3 = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p   = pa[i];
        unsigned mod = fib_mod3[p & 7U]; /* p % 8 */
        sum_mod3 += (unsigned)(ex[i] * mod);
        sum_mod3 %= 3;
    }

    return (seq_t)(sum_mod3 == 0 ? 1 : 0);
}

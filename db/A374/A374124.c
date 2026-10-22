/*
 * A374124  a(n) = A113177(n) mod 360, where A113177 is fully additive with
 *          a(p) = Fibonacci(p).
 * https://oeis.org/A374124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A113177(n) mod 360, where A113177 is fully additive with
 *            a(p) = Fibonacci(p)
 * Range:     u8 n <= 40, u16 n <= 9692, u32 n <= 9692, u64 n <= 9692,
 *            u128 n <= 9692, big any n
 * Helpers:   factor_u64, fibmod360
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/fibmod360.h"

/* Fully additive function A113177(n) modulo 360, then modulo 360 again
   (the result is already <360). */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;          /* sequence starts at n = 1 */

    /* Factor n into primes and exponents. */
    uint64_t prime[64];
    uint64_t exp[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, prime, exp, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t fp = fibmod360(prime[i]);          /* Fibonacci(p) mod 360 */
        seq_t mexp = (seq_t)exp[i] % 360;       /* exponent modulo 360 */
        seq_t prod = mexp * fp;                 /* product in seq_t */
        sum += prod;                            /* accumulate sum */
        sum %= 360;                              /* keep it within mod 360 */
    }
    return sum;
}

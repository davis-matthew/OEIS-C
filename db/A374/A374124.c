/*
 * A374124  a(n) = A113177(n) mod 360, where A113177 is fully additive with
 *          a(p) = Fibonacci(p).
 * https://oeis.org/A374124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A113177(n) mod 360, where A113177 is fully additive with
 *            a(p) = Fibonacci(p)
 * Range:     u8 n <= 40, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 95 terms through n = 9692 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Compute Fibonacci(p) modulo 360 using fast doubling.  The
   Fibonacci argument p is a prime (or any positive integer),
   but the table is kept small because we only need values up to
   360, so native 32‑bit arithmetic suffices. */
static inline seq_t fibmod360(idx_t p)
{
    uint32_t a = 0;   /* F(0) */
    uint32_t b = 1;   /* F(1) */
    /* Process bits of p from most‑significant to least. */
    for (int i = 63; i >= 0; --i) {
        /* Doubling formulas in modulo 360. */
        uint32_t c = (uint32_t)((a * ((2 * b + 360 - a) % 360)) % 360);   /* F(2k)  */
        uint32_t d = (uint32_t)((a * a + b * b) % 360);                    /* F(2k+1) */
        if ((p >> i) & 1U) {
            a = d;                      /* next F(k)   */
            b = (c + d) % 360;          /* next F(k+1) */
        } else {
            a = c;
            b = d;
        }
    }
    return (seq_t)(a % 360);
}

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

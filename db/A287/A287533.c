/*
 * A287533  Fibonacci numbers modulo 20.
 * https://oeis.org/A287533
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n) mod 20
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   mul_mod20
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mul_mod20.h"

/* Compute floor(log2) for unsigned 64‑bit integers. */
static inline int floor_log2_uint64(idx_t v)
{
    int r = -1;
    while (v) {
        ++r;
        v >>= 1;
    }
    return r;
}

/* Compute the nth Fibonacci number modulo 20 using fast doubling. */
static inline seq_t fib_mod20(idx_t n)
{
    const seq_t MOD = (seq_t)20;
    if (n == 0)
        return (seq_t)0;

    seq_t a = (seq_t)0;  /* F(k)   */
    seq_t b = (seq_t)1;  /* F(k+1) */

    int msb = floor_log2_uint64(n);
    for (int i = msb; i >= 0; --i) {
        /* Doubling formulas:
           c = F(2k)   = F(k) * (2*F(k+1) - F(k))
           d = F(2k+1) = F(k)^2 + F(k+1)^2
        */
        seq_t b2 = mul_mod20(b, (seq_t)2);             /* 2*b mod MOD */
        seq_t t1 = (b2 >= a) ? (b2 - a) : (b2 + MOD - a); /* (2*b - a) mod MOD */
        seq_t c = mul_mod20(a, t1);                   /* a * t1 mod MOD */

        seq_t a_sq = mul_mod20(a, a);
        seq_t b_sq = mul_mod20(b, b);
        seq_t d = a_sq + b_sq;
        if (d >= MOD) d -= MOD;                       /* (a^2 + b^2) mod MOD */

        if (((n >> i) & 1) == 0) {
            a = c;
            b = d;
        } else {
            a = d;
            seq_t sum = c + d;
            if (sum >= MOD) sum -= MOD;               /* (c + d) mod MOD */
            b = sum;
        }
    }
    return a;
}

/* Public interval function required by the test harness. */
seq_t a(idx_t n)
{
    return fib_mod20(n);
}

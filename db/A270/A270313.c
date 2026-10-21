/*
 * A270313  Denominator of Fibonacci(n)/n.
 * https://oeis.org/A270313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = n / gcd(Fibonacci(n), n) (n ≥ 1)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses fast‑doubling Fibonacci with
   modulo arithmetic (using 128‑bit intermediate for safety)
   and an Euclidean gcd on 64‑bit values, then returns the
   quotient cast to seq_t. */

#include <stdint.h>
#include "../utils/gcd.h"   /* for seq_t and idx_t types */
#include "../utils/gcd_u64.h"

static inline uint64_t mul_mod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

static void fib_pair_mod(uint64_t n, uint64_t mod, uint64_t *f, uint64_t *f1)
{
    if (n == 0) {
        *f = 0;
        *f1 = 1;
        return;
    }
    uint64_t a, b;
    fib_pair_mod(n >> 1, mod, &a, &b);
    /* a = F(k), b = F(k+1) where k = n/2 */
    uint64_t two_b = (b * 2) % mod;
    uint64_t c = mul_mod(a, (two_b + mod - a) % mod, mod);     /* F(2k)  */
    uint64_t d = (mul_mod(a, a, mod) + mul_mod(b, b, mod)) % mod; /* F(2k+1) */

    if (n & 1) {
        *f = d;                     /* F(2k+1) */
        *f1 = (c + d) % mod;        /* F(2k+2) */
    } else {
        *f = c;                     /* F(2k)   */
        *f1 = d;                    /* F(2k+1) */
    }
}

seq_t a(idx_t n)
{
    /* a(1) = 1, so n=0 is undefined; return 0 for safety */
    if (n == 0)
        return (seq_t)0;

    uint64_t n64 = (uint64_t)n;
    uint64_t fn_mod, fn1_mod;

    /* Compute Fibonacci(n) modulo n */
    fib_pair_mod(n64, n64, &fn_mod, &fn1_mod);

    /* a(n) = n / gcd(Fibonacci(n), n) */
    uint64_t g = gcd_u64(fn_mod, n64);

    return (seq_t)(n64 / g);
}

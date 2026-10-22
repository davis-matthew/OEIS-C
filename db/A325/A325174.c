/*
 * A325174  a(n) = Fibonacci(n)^n mod prime(n).
 * https://oeis.org/A325174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = fib(n)^n mod prime(n)
 * Range:     u8 n <= 56, u16 n <= 6648, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

/* modular multiplication using 128‑bit intermediate to avoid overflow */
static inline uint64_t mulmod(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__int128)a * b % mod);
}

/* fast doubling algorithm to compute Fibonacci(n) modulo mod */
static uint64_t fib_mod(uint64_t n, uint64_t mod)
{
    uint64_t a = 0; /* F(0) */
    uint64_t b = 1; /* F(1) */
    int msb = 63 - __builtin_clzll(n); /* highest set bit */

    for (int i = msb; i >= 0; --i) {
        /* c = F(2k) = a * (2*b - a) */
        uint64_t two_b = (b << 1) % mod;
        uint64_t t = (two_b + mod - a) % mod;  /* (2*b - a) mod mod */
        uint64_t c = mulmod(a, t, mod);

        /* d = F(2k+1) = a^2 + b^2 */
        uint64_t d = (mulmod(a, a, mod) + mulmod(b, b, mod)) % mod;

        if (n & (1ULL << i)) {
            a = d;
            b = (c + d) % mod;
        } else {
            a = c;
            b = d;
        }
    }
    return a;
}

/* a(n) = Fibonacci(n)^n mod prime(n) */
seq_t a(idx_t n)
{
    /* prime(n) is guaranteed to fit in 32 bits for n ≤ 10000 */
    uint64_t prime = (uint64_t)get_prime(n);

    /* compute Fibonacci(n) modulo the prime */
    uint64_t fib = fib_mod((uint64_t)n, prime);

    /* modular exponentiation: fib^n mod prime */
    uint64_t result = 1;
    uint64_t base = fib % prime;
    uint64_t exp = n; /* n fits into uint64_t (idx_t) */

    while (exp) {
        if (exp & 1)
            result = mulmod(result, base, prime);
        base = mulmod(base, base, prime);
        exp >>= 1;
    }

    return (seq_t)result;
}

/*
 * A247091  Least k such that k*n+1 is a prime divisor of Fibonacci(k).
 * https://oeis.org/A247091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that k*n+1 is a prime divisor of
 *            Fibonacci(k)
 * Range:     u8 n <= 5, u16 n <= 46, u32 n <= 46, u64 n <= 500, u128 n <= 500,
 *            big any n
 * Helpers:   is_prime_big, mul_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External utilities */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/mul_mod64.h"

/* Portable multiplication modulo a 64‑bit modulus.  On compilers that
   support 128‑bit arithmetic the product is computed safely in
   __uint128_t.  If 128‑bit arithmetic is unavailable we fall back to
   the mul_mod64 function supplied by the utilities, which is valid
   for 64‑bit types.  When compiled as C++ we assume that seq_t is an
   arbitrary‑precision integer type that overloads * and % and rely on
   those operators. */
#if defined(__SIZEOF_INT128__) || defined(__GNUC__) || defined(__clang__)
static inline seq_t mulmod(seq_t a, seq_t b, seq_t m)
{
    return (seq_t)((__uint128_t)a * b % m);
}
#elif defined(__cplusplus)
static inline seq_t mulmod(seq_t a, seq_t b, seq_t m)
{
    return (a * b) % m;   /* arbitrary‑precision arithmetic */
}
#else
static inline seq_t mulmod(seq_t a, seq_t b, seq_t m)
{
    return (seq_t)mul_mod64((uint64_t)a, (uint64_t)b, (uint64_t)m);
}
#endif

/* Fast doubling algorithm to compute Fib(n) mod m.  The routine
   calculates Fib(n) and Fib(n+1) modulo m. */
static void fib_pair_mod(idx_t n, seq_t mod, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b, c, d;
    fib_pair_mod(n >> 1, mod, &a, &b);

    /* c = F(2k)     = F(k) * (2*F(k+1) - F(k)) */
    /* d = F(2k+1)   = F(k)^2 + F(k+1)^2 */
    seq_t two_b = (2 * b) % mod;
    seq_t temp  = (two_b + mod - a) % mod;
    c = mulmod(a, temp, mod);
    d = (mulmod(a, a, mod) + mulmod(b, b, mod)) % mod;

    if (n & 1) {
        *fn  = d;
        *fn1 = (c + d) % mod;
    } else {
        *fn  = c;
        *fn1 = d;
    }
}

static seq_t fib_mod(idx_t n, seq_t mod)
{
    seq_t fn, fn1;
    fib_pair_mod(n, mod, &fn, &fn1);
    return fn;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n==0, return 0 */
    for (idx_t k = 1; ; ++k) {
        seq_t p = (seq_t)k * (seq_t)n + 1; /* candidate prime */
        if (!is_prime_big(p)) continue;   /* skip non‑primes */
        if (fib_mod(k, p) == 0) return k; /* found minimal k */
    }
}

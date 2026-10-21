/*
 * A275745  P-defects p - N(p) of the congruence y^2 + x*y + y == x^3 + x^2 -
 *          10*x - 10 (mod p) for primes p,where N(p) is the number of
 *          solutions given for p = prime(n) by A275742(n).
 * https://oeis.org/A275745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 650, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = prime(n) – N(p)
 *
 * Here N(p) is the number of solutions (x,y) modulo the prime p,
 * where p = prime(n), of the congruence
 *
 *      y^2 + x*y + y  ≡  x^3 + x^2 – 10x – 10   (mod p)
 *
 * The implementation evaluates N(p) by iterating over all
 * x ∈ {0,…,p-1}, solving the quadratic in y and counting
 * the solutions via the Legendre symbol.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

/* Elementary modular multiplication (uses 128‑bit int, available on GCC/Clang) */
static inline uint64_t mul_mod64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)(((__uint128_t)a * b) % mod);
}

/* Fast modular exponentiation, returns base^exp mod mod */
static inline uint64_t pow_mod64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t r = 1;
    base %= mod;
    while (exp) {
        if (exp & 1) r = mul_mod64(r, base, mod);
        base = mul_mod64(base, base, mod);
        exp >>= 1;
    }
    return r;
}

/* Legendre symbol (a|p), p odd prime */
static inline int legendre_symbol(uint64_t a, uint64_t p)
{
    if (a % p == 0) return 0;                /* divisible by p */
    uint64_t ls = pow_mod64(a % p, (p - 1) / 2, p);
    if (ls == 1) return 1;
    if (ls == p - 1) return -1;
    return 0;                                 /* should not occur for prime p */
}

seq_t a(idx_t n)
{
    /* prime(n) (n starts at 1) */
    uint32_t p32 = get_prime(n);
    uint64_t p = p32;

    /* Special case p = 2 : 3 solutions by direct check */
    if (p == 2ULL) {
        /* N(2) = 3, so a(1) = 2 – 3 = –1 */
        return (seq_t)(-1);
    }

    /* Count the number of solutions N(p) */
    long long solutions = 0;          /* fits comfortably for p up to 10^5 */

    uint64_t ten_mod = 10 % p;
    uint64_t four_mod = 4 % p;

    for (uint64_t x = 0; x < p; ++x) {
        uint64_t x_mod  = x;          /* 0 <= x_mod < p */

        /* x^2 and x^3 modulo p */
        uint64_t x2 = mul_mod64(x_mod, x_mod, p);
        uint64_t x3 = mul_mod64(x2, x_mod, p);

        /* RHS = x^3 + x^2 – 10x – 10  (mod p) */
        uint64_t rhs = (x3 + x2) % p;
        uint64_t term10x = mul_mod64(ten_mod, x_mod, p);
        rhs = (rhs + p - term10x) % p;
        rhs = (rhs + p - ten_mod) % p;

        /* b = x + 1 mod p */
        uint64_t b = (x_mod + 1) % p;
        uint64_t b2 = mul_mod64(b, b, p);

        /* Discriminant D = b^2 + 4 * RHS  (mod p) */
        uint64_t four_r = mul_mod64(four_mod, rhs, p);
        uint64_t D = (b2 + four_r) % p;

        if (D == 0) {                 /* exactly one root */
            solutions += 1;
        } else {
            int leg = legendre_symbol(D, p);
            if (leg == 1)             /* two distinct roots */
                solutions += 2;
            /* leg == -1 gives zero roots – do nothing */
        }
    }

    /* a(n) = p – N(p) */
    long long diff = (long long)p - solutions;
    return (seq_t)diff;
}

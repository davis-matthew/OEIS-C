/*
 * A274164  Smallest k > 0 such that F_{p-(k/p)} == 0 (mod p), where p =
 *          prime(n), F_i = A000045(i) and (a/b) is the Kronecker symbol.
 * https://oeis.org/A274164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   get_prime, kronecker_mod2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A274164  –  smallest k > 0 such that
 *  F_{p - (k/p)} ≡ 0 (mod p)
 *  where p = prime(n) and (·/p) is the Kronecker symbol.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/kronecker_mod2.h"

/* Legendre symbol (a/p) for an odd prime p. */
static inline int legendre(uint64_t a, uint64_t p)
{
    a %= p;
    if (a == 0) return 0;
    uint64_t exp = (p - 1) >> 1;
    uint64_t res = 1;
    uint64_t base = a;
    while (exp) {
        if (exp & 1) res = (res * base) % p;
        base = (base * base) % p;
        exp >>= 1;
    }
    return (res == p - 1) ? -1 : 1;
}

/* Fast doubling algorithm to compute F_n mod m.  n > 0. */
static inline void fib_mod(uint64_t n, uint64_t mod, uint64_t *fn)
{
    uint64_t a = 0;   /* F(0) */
    uint64_t b = 1;   /* F(1) */

    /* Highest set bit of n */
    int top = 63 - __builtin_clzll(n);
    for (int i = top; i >= 0; --i) {
        /* c = F(2k) = F(k) * (2*F(k+1) – F(k)) mod mod */
        uint64_t two_b = (b << 1) % mod;
        uint64_t diff = (two_b + mod - a) % mod;      /* (2*b - a) mod mod */
        uint64_t c = (a * diff) % mod;

        /* d = F(2k+1) = F(k)^2 + F(k+1)^2 mod mod */
        uint64_t a_sq = (a * a) % mod;
        uint64_t b_sq = (b * b) % mod;
        uint64_t d = (a_sq + b_sq) % mod;

        if ((n >> i) & 1) {
            a = d;           /* F(2k+1) */
            b = (c + d) % mod;  /* F(2k+2) */
        } else {
            a = c;           /* F(2k)   */
            b = d;           /* F(2k+1) */
        }
    }
    *fn = a;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Get the n-th prime.   get_prime uses 1‑based indexing. */
    uint64_t p = (uint64_t)get_prime(n);

    /* Search for the smallest k ∈ {1,…,5}.  The theory guarantees
     * such a k exists and is at most 5. */
    for (idx_t k = 1; k <= 5; ++k) {
        int kr;              /* (k/p) */

        if (p == 2) {
            kr = kronecker_mod2(k);
        } else {
            kr = legendre(k, p);
        }

        /* Index in the Fibonacci sequence:  p - (k/p)  */
        uint64_t idx = (kr == 1) ? (p - 1) : ((kr == -1) ? (p + 1) : p);

        uint64_t fn;
        fib_mod(idx, p, &fn);

        if (fn == 0) return (seq_t)k;
    }

    /* Theoretical guarantee ensures we never reach here. */
    return (seq_t)5;
}

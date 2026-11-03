/*
 * A325174  a(n) = Fibonacci(n)^n mod prime(n).
 * https://oeis.org/A325174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = fib(n)^n mod prime(n)
 * Range:     u8 n <= 56, u16 n <= 6648, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 86 terms through n = 9641 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Return the k‑th prime (k ≥ 1).  On first call the prime list
   is built incrementally using trial division.  The list is
   stored in a static array which is expanded as necessary.   */
static uint32_t get_prime(idx_t k)
{
    static uint32_t *pr = NULL;     /* array of primes */
    static size_t sz   = 0;         /* number of primes stored */
    static size_t cap  = 0;         /* allocated capacity */
    static uint32_t cand = 3;       /* candidate after 2 */

    /* initialise the prime table on first use */
    if (sz == 0) {
        cap = 8;
        pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
        if (!pr) abort();
        sz = 0;
    }

    /* grow the array until we have at least k primes */
    while (sz < (size_t)k) {
        if (sz == 0) {                /* add the first prime */
            pr[sz++] = 2;
            continue;
        }

        /* find the next prime candidate */
        uint32_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint32_t prime = pr[i];
            if ((uint64_t)prime * prime > (uint64_t)p) break;
            if (p % prime == 0) { ok = false; break; }
        }

        if (!ok) {                    /* not prime – keep searching */
            cand += 2;                /* only odd numbers remain */
            continue;
        }

        /* prime found – store it, grow table if needed */
        if (sz == cap) {
            cap <<= 1;
            pr = (uint32_t *)realloc(pr, cap * sizeof(uint32_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
        cand = p + 2;
    }

    return pr[(size_t)k - 1];
}

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

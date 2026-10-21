/*
 * A245526  Unique integer r with -prime(n)/2 < r <= prime(n)/2 such that
 *          L(2*n) == r (mod prime(n)), where L(k) denotes the Lucas number
 *          A000032(k).
 * https://oeis.org/A245526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = r where -prime(n)/2 < r <= prime(n)/2 and Lucas(2n) ≡ r
 *            (mod prime(n))
 * Range:     i8 n <= 32, i16 n <= 3815, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   get_prime, mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/mod_mul.h"

/* Small helpers for modular arithmetic modulo a small prime p (uint32_t). */
static inline seq_t mod_sub(seq_t a, seq_t b, uint32_t mod)
{
    return a >= b ? a - b : a + mod - b;
}

static inline seq_t mod_add(seq_t a, seq_t b, uint32_t mod)
{
    seq_t s = a + b;
    if (s >= mod) s -= mod;
    return s;
}

static inline seq_t mod_mul_uint32(seq_t a, seq_t b, uint32_t mod)
{
    return mod_mul((uint64_t)a, (uint64_t)b, (uint64_t)mod);
}

/* Fast doubling algorithm to compute (F(n), F(n+1)) modulo mod. */
static void fib_pair_mod(idx_t n, uint32_t mod, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    seq_t a, b;
    fib_pair_mod(n >> 1, mod, &a, &b);          /* a=F(k), b=F(k+1) */
    seq_t two_b = b + b;
    if (two_b >= mod) two_b -= mod;              /* 2*b % mod */
    seq_t t = mod_sub(two_b, a, mod);           /* (2*b - a) % mod */
    seq_t f2k   = mod_mul_uint32(a, t, mod);     /* F(2k)   */
    seq_t f2k1  = mod_add(mod_mul_uint32(a, a, mod),
                         mod_mul_uint32(b, b, mod), mod); /* F(2k+1) */
    if ((n & 1ULL) == 0) {
        *fn  = f2k;
        *fn1 = f2k1;
    } else {
        *fn  = f2k1;
        /* F(2k+2) = F(2k) + F(2k+1) */
        *fn1 = mod_add(f2k, f2k1, mod);
    }
}

seq_t a(idx_t n)
{
    /* Prime number p = prime(n). */
    uint32_t p = get_prime(n);

    /* Compute Lucas(2n) mod p. */
    idx_t k = n << 1;            /* k = 2n */
    seq_t fn, fn1;               /* Fibonacci numbers F(k) and F(k+1) */
    fib_pair_mod(k, p, &fn, &fn1);

    /* Lucas(n) ≡ 2·F(n+1) – F(n) (mod p). */
    seq_t luc_raw = mod_sub(mod_mul_uint32(2, fn1, p), fn, p); /* raw residue [0,p-1] */

    /* Adjust to signed window: -p/2 < r <= p/2. */
    seq_t r;
    if (luc_raw <= (seq_t)(p >> 1))
        r = luc_raw;                 /* non‑negative side */
    else
        r = luc_raw - (seq_t)p;      /* negative side (p is odd for primes >2) */

    return r;
}

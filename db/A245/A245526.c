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
 * Validated: standard, pass, 96 terms through n = 9884 (2026-11-01)
 * Helpers:   mulmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mulmod.h"

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
    return mulmod((uint64_t)a, (uint64_t)b, (uint64_t)mod);
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

/*
 * A395605  a(n) = ((Lucas(prime(n)) - 1) / prime(n)) mod prime(n).
 * https://oeis.org/A395605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = ((Lucas(prime(n)) - 1) / prime(n)) mod prime(n)
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 6399, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9934 (2026-11-01)
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

/* modular multiplication: (a * b) % mod for any seq_t type */
static inline seq_t mod_mul_seq(seq_t a, seq_t b, seq_t mod) {
    seq_t res = 0;
    /* binary multiplication – safe for uint64_t and arbitrary‑precision types */
    while (b) {
        if (b & 1) res = (res + a) % mod;
        a = (a << 1) % mod;
        b >>= 1;
    }
    return res;
}

/* fast doubling to compute F(n) and F(n+1) modulo mod */
static void fib_mod_u64(idx_t n, seq_t mod, seq_t *fn, seq_t *fn1) {
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    fib_mod_u64(n >> 1, mod, fn, fn1);
    seq_t a = *fn;    /* F(k)   */
    seq_t b = *fn1;   /* F(k+1) */

    seq_t two_b   = (b + b) % mod;          /* 2*F(k+1) mod mod */
    seq_t t_minus = (two_b + mod - a) % mod;/* (2*F(k+1)-F(k)) */
    seq_t c = mod_mul_seq(a, t_minus, mod);/* F(2k) */

    seq_t a2 = mod_mul_seq(a, a, mod);
    seq_t b2 = mod_mul_seq(b, b, mod);
    seq_t d = (a2 + b2) % mod;              /* F(2k+1) */

    if (n & 1) {
        *fn  = d;
        *fn1 = (c + d) % mod;
    } else {
        *fn  = c;
        *fn1 = d;
    }
}

/* sequence term */
seq_t a(idx_t n) {
    /* nth prime (1‑based) */
    uint64_t p_uint = get_prime(n);
    seq_t p = (seq_t)p_uint;
    seq_t mod = p * p;                    /* modulo p² */

    /* Lucas(p) modulo p² : Lucas(p) = 2*F(p+1) – F(p) */
    seq_t fp, fpp;
    fib_mod_u64(p_uint, mod, &fp, &fpp);

    seq_t two_fpp   = (fpp + fpp) % mod;
    seq_t lucas_mod = (two_fpp + mod - fp) % mod; /* 0 ≤ lucas_mod < mod */

    /* (Lucas(p) – 1) / p  (exact division) */
    seq_t r = (lucas_mod - 1) / p;
    return r;
}

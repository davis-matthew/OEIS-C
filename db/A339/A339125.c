/*
 * A339125  Odd composite integers m such that A000032(m-J(m,5)) == 2*J(m,5)
 *          (mod m), where J(m,5) is the Jacobi symbol.
 * https://oeis.org/A339125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = odd composite m for which A000032(m - J(m,5)) ≡ 2·J(m,5)
 *            (mod m) (J is the Jacobi symbol)
 * Range:     u8 n <= 4, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   isPrimeTrial, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/jacobi_symbol.h"
#include "../utils/isPrimeTrial.h"

/* Multiply modulo mod using 128‑bit intermediate (sufficient for seq_t ≤ 64‑bit). */
static inline seq_t mulmod(seq_t a, seq_t b, seq_t mod)
{
    return (seq_t)((unsigned __int128)a * b % mod);
}

/* fast‑doubling Fibonacci: returns F(n), F(n+1) modulo mod. */
static void fib_pair(uint64_t n, seq_t mod, seq_t *fn, seq_t *fn1)
{
    if (n == 0) {
        *fn  = 0;
        *fn1 = 1 % mod;
        return;
    }
    seq_t a, b;
    fib_pair(n >> 1, mod, &a, &b);          /* a = F(k), b = F(k+1), k = ⌊n/2⌋ */
    seq_t t  = (seq_t)((unsigned __int128)b * 2 % mod); /* 2*F(k+1) */
    seq_t c  = mulmod(a, (t + mod - a) % mod, mod);    /* F(2k)   */
    seq_t d  = (mulmod(a, a, mod) + mulmod(b, b, mod)) % mod;  /* F(2k+1) */
    if ((n & 1) == 0) {
        *fn  = c;
        *fn1 = d;
    } else {
        *fn  = d;
        *fn1 = (c + d) % mod;
    }
}

/* Lucas number L(k) modulo mod.  Uses L(k)=2*F(k+1)-F(k). */
static seq_t lucas_mod(seq_t k, seq_t mod)
{
    seq_t fk, fk1;
    fib_pair((uint64_t)k, mod, &fk, &fk1);
    seq_t two_fk1 = (seq_t)((unsigned __int128)fk1 * 2 % mod);
    return (two_fk1 + mod - fk) % mod;
}

/* Cache to avoid repeated work for successive calls. */
static seq_t *cache = NULL;      /* array of results */
static size_t cache_len = 0;     /* number of cached terms */
static size_t cache_cap = 0;     /* allocated capacity */
static seq_t next_search_m = 3;  /* odd integer to start searching from */

static void ensure_capacity(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *newp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = newp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Under‑offset behaviour is undefined; return 0 for safety. */
        return (seq_t)0;
    }
    /* Extend cache until it contains at least n terms. */
    while (cache_len < (size_t)n) {
        /* Scan odd numbers until we find the next qualifying composite. */
        seq_t m = next_search_m;
        for (;;) {
            /* Skip primes and numbers divisible by 5 (Jacobi symbol would be 0). */
            if ((m % 5) == 0 || isPrimeTrial(m)) {
                m += 2;
                continue;
            }
            int j = jacobi_symbol((uint64_t)m, 5);   /* J(m,5) */
            if (j == 0) { /* Should not happen because we skipped multiples of 5, but keep safe */
                m += 2;
                continue;
            }
            seq_t k = (j == 1) ? m - 1 : m + 1;      /* m - J(m,5) */
            seq_t luc = lucas_mod(k, m);
            seq_t target = (j == 1) ? 2 : m - 2;     /* 2*J(m,5) modulo m */
            if (luc == target) {
                /* Accept this m. */
                ensure_capacity(cache_len + 1);
                cache[cache_len++] = m;
                next_search_m = m + 2; /* continue from next odd number */
                break;
            }
            m += 2;
        }
    }
    return cache[n - 1];
}

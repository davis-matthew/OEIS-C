/*
 * A308784  Primes p such that A001175(p) = 2*(p+1)/3.
 * https://oeis.org/A308784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 464, u32 n <= 714, u64 n <= 714,
 *            u128 n <= 714, big any n
 * Helpers:   is_prime, jacobi_symbol, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the n‑th term of OEIS A308784: primes p such that
 *  A001175(p) = 2*(p+1)/3.
 *  The function searches primes in increasing order, testing each
 *  prime with the Pisano‑period (Fibonacci) condition.
 *  A static cache allows arbitrary – order calls to be fast.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Utility headers provided by the build system */
#include "../utils/is_prime.h"          /* bool is_prime(idx_t x)          */
#include "../utils/next_prime.h"        /* uint64_t next_prime(uint64_t)  */
#include "../utils/jacobi_symbol.h"     /* int  jacobi_symbol(uint64_t a,uint64_t n) */

/* ---- fast doubling Fibonacci modulo mod ---------------------------------- */
static void fib_mod(uint64_t n, uint64_t mod, uint64_t *fn, uint64_t *fn1)
{
    uint64_t a = 0;   /* F(0) */
    uint64_t b = 1;   /* F(1) */

    if (n == 0) { *fn = 0; *fn1 = 1; return; }

    int bit = 63;
    while (((uint64_t)1 << bit) > n) --bit;

    for (; bit >= 0; --bit) {
        /* c = F(2k)      = F(k) * [2*F(k+1) – F(k)] */
        uint64_t t = ((unsigned __int128)2 * b) % mod;
        t = (t + mod - a) % mod;                  /* 2*b - a (mod mod) */
        uint64_t c = ((unsigned __int128)a * t) % mod;

        /* d = F(2k+1)    = F(k)^2 + F(k+1)^2 */
        uint64_t d = ( (unsigned __int128)a * a +
                       (unsigned __int128)b * b ) % mod;

        if (n & ((uint64_t)1 << bit)) {
            a = d;
            b = (c + d) % mod;
        } else {
            a = c;
            b = d;
        }
    }
    *fn  = a;
    *fn1 = b;
}

/* ---- recursive enumeration of proper divisors of N ------------------------ */
static bool has_smaller_divisor(uint64_t N, uint64_t mod,
                                const uint64_t *pf, const uint32_t *exp,
                                int pc, int idx, uint64_t cur)
{
    if (idx == pc) {
        if (cur == 1 || cur == N) return false;   /* ignore trivial divisors */
        uint64_t f, f1;
        fib_mod(cur, mod, &f, &f1);
        return (f == 0 && f1 == 1);
    }

    uint64_t val = 1;
    for (uint32_t e = 0; e <= exp[idx]; ++e) {
        if (has_smaller_divisor(N, mod, pf, exp, pc, idx + 1, cur * val))
            return true;
        val *= pf[idx];
    }
    return false;
}

/* ---- test whether a prime satisfies the required condition ---------------- */
static bool is_special_prime(uint64_t p)
{
    /* The primes in the sequence satisfy p ≡ 2 (mod 3) */
    if (p % 3u != 2u) return false;
    /* Jacobi symbol (5/p) must be -1 (5 is a quadratic non‑residue) */
    if (jacobi_symbol(5, p) != -1) return false;

    /* Desired Pisano period for this prime */
    uint64_t N = 2 * (p + 1) / 3;

    /* Verify that F_N ≡ 0 and F_{N+1} ≡ 1 (mod p) */
    uint64_t fN, fN1;
    fib_mod(N, p, &fN, &fN1);
    if (!(fN == 0 && fN1 == 1)) return false;

    /* Factor N and check for any proper divisor giving the same period */
    uint64_t tmp = N;
    uint64_t pf[64];
    uint32_t  exp[64];
    int pc = 0;

    for (uint64_t d = 2; d * d <= tmp; ++d) {
        if (tmp % d == 0) {
            uint32_t e = 0;
            while (tmp % d == 0) { tmp /= d; ++e; }
            pf[pc] = d;  exp[pc] = e;  ++pc;
        }
    }
    if (tmp > 1) { pf[pc] = tmp; exp[pc] = 1; ++pc; }

    return !has_smaller_divisor(N, p, pf, exp, pc, 0, 1);
}

/* ---- main sequence function ------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Static cache for found primes */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static uint64_t last_checked = 1;   /* prime just processed */

    if (!cache) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    while (cache_len < (size_t)n) {
        uint64_t p = next_prime(last_checked);
        last_checked = p;
        if (!is_special_prime(p)) continue;

        if (cache_len == cache_cap) {
            cache_cap <<= 1;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[cache_len++] = (seq_t)p;
    }
    return cache[n - 1];
}

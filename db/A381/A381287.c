/*
 * A381287  a(n) is the smallest nonnegative number congruent to k modulo
 *          prime(k)^(n-k+1) for k=1..n.
 * https://oeis.org/A381287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest nonnegative integer x such that * for all k
 *            = 1..n, x ≡ k (mod prime(k)^(n-k+1))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation uses the Chinese Remainder Theorem and
 *  a quick extended‑Euclid linear congruence solver.          */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/big_pow.h"
#include "../utils/build_primes_upto.h"

/* ---------- helper: modular inverse with non‑negative coefficients ---------- */

static seq_t mod_inv(seq_t a, seq_t m)
{
    seq_t a0 = a % m;
    seq_t b0 = m;
    seq_t x0 = 1 % m;
    seq_t x1 = 0;

    while (b0 != 0) {
        seq_t q = a0 / b0;
        seq_t r = a0 % b0;
        a0 = b0;  b0 = r;

        /* q * x1  (mod m) */
        seq_t qx1_mod = (q * x1) % m;
        /* (x0 - q*x1) mod m, avoiding negative intermediate */
        seq_t t = (x0 + m - qx1_mod) % m;

        x0 = x1;
        x1 = t;
    }
    return (a0 == 1) ? x0 : 0;   /* return 0 if inverse does not exist */
}

/* ---------- prime cache management --------------------------------------- */

static uint64_t *prime_cache = NULL;
static size_t   prime_cache_cnt = 0;

static void ensure_primes(idx_t n)
{
    if (prime_cache_cnt >= (size_t)n)
        return;

    /* free old cache (if any) */
    free(prime_cache);
    prime_cache = NULL;
    prime_cache_cnt = 0;

    idx_t limit = (n < 6) ? 15 : 20 * n + 10;   /* rough upper bound on nth prime */

    while (1) {
        uint64_t *pr = NULL;
        size_t cnt = 0;
        build_primes_upto((idx_t)limit, &pr, &cnt);
        if (cnt >= (size_t)n) {
            prime_cache = pr;
            prime_cache_cnt = cnt;
            break;
        }
        /* insufficient primes – increase limit and retry */
        free(pr);
        limit *= 2;
    }
}

/* ---------- sequence computation ----------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0)   /* sequence is defined for n >= 1, but guard anyway */
        return 0;

    ensure_primes(n);

    seq_t x = 0;            /* current solution modulo M */
    seq_t M = 1;            /* current modulus */

    for (idx_t k = 1; k <= n; ++k) {
        seq_t p_k = (seq_t)prime_cache[k - 1];          /* k-th prime */

        idx_t exp_k = n - k + 1;                       /* exponent for this prime */

        seq_t m_k = big_pow(p_k, (seq_t)exp_k);        /* p_k ^ exp_k */
        seq_t a_k = (seq_t)k;                          /* required residue */

        seq_t M_mod_mk = M % m_k;                      /* M modulo current modulus */

        seq_t inv = mod_inv(M_mod_mk, m_k);            /* inverse of M modulo m_k */
        if (inv == 0)                                 /* safety – should never happen */
            return 0;

        seq_t diff = (a_k + m_k - (x % m_k)) % m_k;    /* difference modulo m_k */
        seq_t t    = (diff * inv) % m_k;               /* CRT adjustment term */

        x = x + M * t;                                 /* new solution */
        M = M * m_k;                                   /* new modulus */
    }

    return x;
}

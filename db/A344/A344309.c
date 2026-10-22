/*
 * A344309  a(n) is the number of eigensequences (including eigenvalues, where
 *          they exist) of the Fibonacci recurrence in Zp where p is prime(n).
 * https://oeis.org/A344309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   build_primes_upto, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A344309: a(n) = E(p)/(p-1) where p = prime(n)
   E(p) = 4                                 if p = 5
          (p-1)^2 / ep + (p-1)             if p ≡ 1 or 4 (mod 5)
          (p^2-1) / ep + 1 - p             if p ≡ 2 or 3 (mod 5)
   ep = the rank of apparition of p in the Fibonacci sequence
*/
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/get_prime.h"
#include "../utils/build_primes_upto.h"

/* ------------------------------------------------------------------ */
/* Fast doubling Fibonacci modulo m                                    */
/* ------------------------------------------------------------------ */
static inline void fib_pair(uint64_t n, uint64_t m, uint64_t *fn, uint64_t *fn1)
{
    if (n == 0)
    {
        *fn  = 0;
        *fn1 = 1;
        return;
    }
    uint64_t a, b, c, d;
    fib_pair(n >> 1, m, &a, &b);

    uint64_t two_b = (b << 1) % m;
    uint64_t tmp   = (two_b >= a) ? (two_b - a) : (two_b + m - a);
    c = (a * tmp) % m;
    d = ((a * a) + (b * b)) % m;

    if ((n & 1) == 0)
    {
        *fn  = c;
        *fn1 = d;
    }
    else
    {
        *fn  = d;
        *fn1 = (c + d) % m;
    }
}

/* ------------------------------------------------------------------ */
/* Prime list used for trivial factorisation of small numbers          */
/* ------------------------------------------------------------------ */
static uint64_t *small_primes = NULL;
static size_t    sp_cnt       = 0;

static void init_small_primes(void)
{
    if (sp_cnt > 0)
        return;
    /* Upper bound sufficient for all M in this problem. */
    const idx_t limit = 1000;
    build_primes_upto(limit, (uint64_t **)&small_primes, &sp_cnt);
}

/* ------------------------------------------------------------------ */
/* Generate all divisors of n from its prime factorisation            */
/* ------------------------------------------------------------------ */
static void gen_divisors(uint64_t n,
                         const uint64_t *primes, const int *exps, size_t pf_cnt,
                         uint64_t *divs, size_t *div_cnt)
{
    *div_cnt = 1;
    divs[0]   = 1;

    for (size_t i = 0; i < pf_cnt; ++i)
    {
        uint64_t p   = primes[i];
        int      e   = exps[i];
        size_t   cur = *div_cnt;
        uint64_t pk = 1;
        for (int k = 1; k <= e; ++k)
        {
            pk *= p;
            for (size_t j = 0; j < cur; ++j)
                divs[(*div_cnt)++] = divs[j] * pk;
        }
    }
}

/* ------------------------------------------------------------------ */
/* Compare function for qsort                                          */
/* ------------------------------------------------------------------ */
static int cmp_u64(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a;
    uint64_t y = *(const uint64_t *)b;
    return (x < y) ? -1 : (x > y);
}

/* ------------------------------------------------------------------ */
/* Compute the rank of apparition of a prime p in the Fibonacci seq.  */
/* (the smallest m>0 with Fib(m)≡0 mod p)                            */
/* ------------------------------------------------------------------ */
static uint64_t rank_apparition(uint64_t p)
{
    if (p == 5ULL)
        return 5ULL;          /* Fib(5) = 5 */

    /* Upper bound for the rank.  */
    uint64_t M = (p % 5 == 1 || p % 5 == 4) ? (p - 1) : (2 * (p + 1));

    /* Factorise M using the small primes list. */
    init_small_primes();
    uint64_t pf[20];
    int      exps[20];
    size_t   pf_cnt = 0;
    uint64_t t = M;

    for (size_t i = 0; i < sp_cnt && (uint64_t)small_primes[i] * small_primes[i] <= t; ++i)
    {
        uint64_t q = small_primes[i];
        if (t % q == 0)
        {
            int e = 0;
            while (t % q == 0)
            {
                t /= q;
                ++e;
            }
            pf[pf_cnt]   = q;
            exps[pf_cnt] = e;
            ++pf_cnt;
        }
    }
    if (t > 1)
    {
        pf[pf_cnt]   = t;
        exps[pf_cnt] = 1;
        ++pf_cnt;
    }

    /* Enumerate all divisors of M. */
    uint64_t divs[2048];
    size_t   div_cnt;
    gen_divisors(M, pf, exps, pf_cnt, divs, &div_cnt);
    qsort(divs, div_cnt, sizeof(uint64_t), cmp_u64);

    /* Find the smallest divisor d with Fib(d)≡0 (mod p). */
    uint64_t dummy;
    for (size_t i = 0; i < div_cnt; ++i)
    {
        uint64_t d;
        fib_pair(divs[i], p, &d, &dummy); /* dummy receives Fn+1 */
        if (d == 0)
            return divs[i];
    }
    /* Fallback: should never happen. */
    return M;
}

/* ------------------------------------------------------------------ */
/* Public function computing a(n) for the n-th prime.                  */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* defensive, offset is 1 */

    uint64_t p = (uint64_t)get_prime(n); /* n-th prime (1‑based) */

    /* Compute ep = rank of apparition of p. */
    uint64_t ep = rank_apparition(p);

    /* Compute E(p) according to the residue class of p modulo 5. */
    uint64_t E;
    if (p == 5ULL)
    {
        E = 4ULL;
    }
    else if (p % 5 == 1 || p % 5 == 4)
    {
        uint64_t t = (p - 1);
        E = (t * t) / ep + t;
    }
    else /* p % 5 == 2 or 3 */
    {
        uint64_t t = p * p;
        E = (t - 1) / ep + 1 - p;
    }

    /* a(n) = E(p)/(p-1). All divisions are exact. */
    return (seq_t)(E / (p - 1));
}

/*
 * A294928  Number of proper divisors of n that are nonabundant (A263837).
 * https://oeis.org/A294928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 523, u16 n <= 9511, u32 n <= 9511, u64 n <= 9511,
 *            u128 n <= 9511, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A294928: number of proper divisors of n that are non‑abundant
 * A proper divisor d of n is non‑abundant iff σ(d) ≤ 2·d,
 * where σ is the sum of divisors function.
 * This function implements that definition via a small prime‑factor sieve
 * and recursion over the divisors.
 * In particular: a(n) = #{ d | n, d < n , σ(d) ≤ 2·d } .
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Forward declaration */
seq_t a(idx_t n);

/* --------------------------------------------------------------------- */
/* static cache for divisor sums σ and smallest prime factors (spf)     */
/* --------------------------------------------------------------------- */
static seq_t *sigma_tbl = NULL;   /* σ(0)…σ(limit) */
static uint64_t *spf_tbl  = NULL; /* spf(0)…spf(limit) */
static idx_t     max_limit = 0;   /* highest pre‑computed index */

/* --------------------------------------------------------------------- */
/* ensure that tables cover at least 'limit' (inclusive)                */
/* --------------------------------------------------------------------- */
static void
ensure_limit(idx_t limit)
{
    if (limit <= max_limit)
        return;                /* already sufficient */

    /* enlarge sigma table */
    seq_t *new_sigma = (seq_t *)realloc(sigma_tbl,
                                        (size_t)(limit + 1) * sizeof(seq_t));
    if (!new_sigma) exit(EXIT_FAILURE);
    sigma_tbl = new_sigma;

    /* enlarge spf table */
    uint64_t *new_spf = (uint64_t *)realloc(spf_tbl,
                                            (size_t)(limit + 1) * sizeof(uint64_t));
    if (!new_spf) exit(EXIT_FAILURE);
    spf_tbl = new_spf;

    /* initialise sigma */
    sigma_tbl[0] = 0;
    for (idx_t i = 1; i <= limit; ++i)
        sigma_tbl[i] = 1;               /* divisor 1 */

    /* compute σ by adding each divisor */
    for (idx_t i = 2; i <= limit; ++i)
        for (idx_t j = i; j <= limit; j += i)
            sigma_tbl[j] += (seq_t)i;

    /* initialise spf */
    for (idx_t i = 0; i <= limit; ++i)
        spf_tbl[i] = i;
    spf_tbl[0] = spf_tbl[1] = 1;           /* special cases */

    /* sieve smallest prime factor */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (spf_tbl[i] == i) {             /* i is prime */
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf_tbl[j] == j)
                    spf_tbl[j] = i;
        }
    }

    max_limit = limit;
}

/* --------------------------------------------------------------------- */
/* recursive generation of all divisors of n (except n)                  */
/* count those that are non‑abundant (σ(d) <= 2·d)                        */
/* --------------------------------------------------------------------- */
static void
gen_divs_rec(idx_t idx,
             int   num_primes,
             idx_t primes[],
             int   exps[],
             idx_t cur,      /* current divisor */
             seq_t *cnt,
             idx_t n)
{
    if (idx == num_primes) {                 /* finished a divisor */
        if (cur < n) {                       /* proper divisor only */
            seq_t limit2 = (seq_t)2 * (seq_t)cur;
            if (sigma_tbl[cur] <= limit2)
                ++(*cnt);
        }
        return;
    }

    const idx_t p = primes[idx];
    const int   e = exps[idx];

    idx_t cur_mul = cur;
    for (int k = 0; k <= e; ++k) {           /* try p^k */
        gen_divs_rec(idx + 1, num_primes,
                     primes, exps, cur_mul,
                     cnt, n);
        if (k == e) break;
        cur_mul *= p;                       /* next power of p */
    }
}

/* --------------------------------------------------------------------- */
/* factorise n using the precomputed spf table                          */
/* --------------------------------------------------------------------- */
static void
factorize(idx_t n,
          idx_t primes[], int exps[], int *num_out)
{
    int num = 0;
    while (n > 1) {
        uint64_t p = spf_tbl[n];
        int cnt = 0;
        while (n % p == 0) {
            n /= p;
            ++cnt;
        }
        primes[num] = (idx_t)p;
        exps[num]   = cnt;
        ++num;
    }
    *num_out = num;
}

/* --------------------------------------------------------------------- */
/* main sequence function                                                */
/* --------------------------------------------------------------------- */
seq_t
a(idx_t n)
{
    if (n < 1) return 0;          /* defined only for n ≥ 1 */

    /* pre‑compute tables up to n */
    ensure_limit(n);

    /* prime factorisation */
    idx_t  primes[64];
    int    exps[64];
    int    num_primes;
    factorize(n, primes, exps, &num_primes);

    /* recurse over all divisors and count non‑abundant ones */
    seq_t cnt = 0;
    gen_divs_rec(0, num_primes, primes, exps, 1, &cnt, n);

    return cnt;
}

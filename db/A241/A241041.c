/*
 * A241041  Differences between primes having smallest primitive root 3.
 * https://oeis.org/A241041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 3282, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A241041: differences between consecutive primes whose
   smallest primitive root is 3.
   a(n) = p_{k+1} – p_k  for the k‑th qualifying prime (offset 1). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Dependencies ------------------------------------------------------- */
/* next_prime(uint64_t start) → first prime ≥ start (works for big ints) */
#include "../utils/next_prime.h"
/* factor_u64(uint64_t n, uint64_t prime[64], uint64_t exp[64], size_t *cnt) */
#include "../utils/factor_u64.h"
/* mod_pow(seq_t base, seq_t exp, seq_t mod) → (base^exp mod mod) */
#include "../utils/mod_pow.h"

/* Helper to test whether g is a primitive root modulo p (p a prime). */
static bool is_primitive_root(uint64_t p, uint64_t g)
{
    if (g % p == 0) return false;
    uint64_t phi = p - 1;

    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64(phi, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t q   = primes[i];
        uint64_t exp = phi / q;
        if (mod_pow((seq_t)g, exp, (seq_t)p) == (seq_t)1)
            return false;   /* order divides phi / q */
    }
    return true;
}

/* Cache ----------------------------------------------------------------- */
static seq_t *diffs   = NULL;   /* stored differences */
static size_t diff_cap = 0;      /* malloc capacity of diffs */
static size_t diff_cnt = 0;      /* number of differences stored */
static uint64_t prev_qprime = 0;/* last qualifying prime found */
static size_t    qprime_cnt  = 0;/* number of qualifying primes found */

/* Ensure that at least n_qual qualifying primes are found. */
static void grow_cache(idx_t n_qual)
{
    /* we need at least n_qual qualifying primes → n_qual‑1 differences   */
    if (qprime_cnt >= (size_t)n_qual) return;

    while (qprime_cnt < (size_t)n_qual)
    {
        /* find next candidate prime */
        uint64_t candidate = (prev_qprime == 0 ? 2 : next_prime(prev_qprime + 1));
        while (candidate <= 3 ||
               !is_primitive_root(candidate, 3) ||
               is_primitive_root(candidate, 2))
        {
            candidate = next_prime(candidate + 1);
        }

        if (qprime_cnt == 0)
        {
            /* first qualifying prime – nothing to store yet */
            prev_qprime = candidate;
        }
        else
        {
            /* store difference with previous qualifying prime */
            if (diff_cnt >= diff_cap)
            {
                size_t new_cap = diff_cap == 0 ? 16 : diff_cap << 1;
                seq_t *tmp = (seq_t *)realloc(diffs, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                diffs = tmp;
                diff_cap = new_cap;
            }
            diffs[diff_cnt++] = (seq_t)candidate - (seq_t)prev_qprime;
            prev_qprime = candidate;
        }

        ++qprime_cnt;
    }
}

/* Main sequence function ------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* below offset – undefined */

    /* need n qualifying primes → n‑1 differences            */
    grow_cache(n + 1);

    return diffs[n - 1];
}

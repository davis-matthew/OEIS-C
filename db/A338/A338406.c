/*
 * A338406  Numbers m such that tau(m), sigma(m) and pod(m) are pairwise
 *          relatively prime.
 * https://oeis.org/A338406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 686, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, gcd, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function generates sequence A338406 by testing integers of the form
   m = r² and checking that τ(m), σ(m) and pod(m) are pairwise relatively prime. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

static seq_t *seq = NULL;
static size_t   seq_len = 0;
static size_t   seq_cap = 0;
static idx_t    checked_max = 0;

/* Append a new term to the cache, resizing if needed. */
static void add_term(seq_t val)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 4;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(*seq));
        seq_cap = new_cap;
    }
    seq[seq_len++] = val;
}

/* Return true if m = r² satisfies that τ(m), σ(m) and pod(m) are pairwise coprime. */
static bool qualifies(uint64_t r)
{
    uint64_t primes[64];
    uint64_t exps  [64];
    size_t   cnt   = 0;
    factor_u64(r, primes, exps, &cnt);   /* factor r */

    seq_t tau   = (seq_t)1;
    seq_t sigma = (seq_t)1;
    seq_t pod   = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        /* exponent of p in m is 2*exps[i] */
        idx_t exp_plus_one = (idx_t)(2 * exps[i] + 1);          /* (e+1) = (2f+1) */
        tau   *= (seq_t)exp_plus_one;                         /* τ(m) */
        pod   *= (seq_t)primes[i];                            /* pod(m) */

        seq_t pow = seq_pow((seq_t)primes[i], exp_plus_one);   /* p^(2f+1) */
        seq_t sigma_factor = (pow - (seq_t)1) / ((seq_t)primes[i] - (seq_t)1);
        sigma *= sigma_factor;                                /* σ(m) */
    }

    if (gcd(tau, sigma) != (seq_t)1) return false;
    if (gcd(tau, pod)   != (seq_t)1) return false;
    /* σ(m) and pod(m) are automatically coprime for any m, but we check for safety */
    if (gcd(sigma, pod) != (seq_t)1) return false;
    return true;
}

seq_t a(idx_t n)
{
    if (!seq) {
        seq_len      = 0;
        seq_cap      = 0;
        seq          = NULL;
        checked_max  = 0;
    }

    while (seq_len < n) {
        ++checked_max;                      /* try next r */
        if (qualifies(checked_max)) {
            seq_t val = (seq_t)checked_max * (seq_t)checked_max; /* m = r² */
            add_term(val);
        }
    }
    return seq[n - 1];                       /* 1‑based indexing */
}

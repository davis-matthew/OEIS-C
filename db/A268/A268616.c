/*
 * A268616  Least squarefree primitive root of the n-th prime.
 * https://oeis.org/A268616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 54, u16 n <= 7985, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf, get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Least squarefree primitive root of the n‑th prime. */
/* a(n) = the smallest g > 1 such that g is a primitive root modulo p(n)
 * and g is square‑free.  a(1) = 1 (the trivial root for p=2).  */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/build_spf.h"
#include "../utils/mod_pow.h"

/*------------------------------------------------------------*/
/*  Small‑prime‑factor sieve (static, expands as needed).     */
/*------------------------------------------------------------*/
static idx_t *spf = NULL;        /* spf[i] = smallest prime factor of i ≥ 2 */
static idx_t spf_limit = 0;      /* current size of allocated spf array */

static void ensure_spf(idx_t limit)
{
    if (limit <= spf_limit) return;
    free(spf);
    spf = NULL;
    build_spf(limit, &spf);
    spf_limit = limit;
}

/* Return up to 32 distinct prime factors of n (n ≥ 2). */
static void distinct_factors(idx_t n, idx_t *out, size_t *cnt)
{
    *cnt = 0;
    while (n > 1) {
        idx_t q = spf[n];
        if (q == 0) q = n;          /* safety, though spf should cover n */
        out[(*cnt)++] = q;
        while (n % q == 0) n /= q;
    }
}

/* Check whether x is square‑free (x ≤ spf_limit). */
static bool is_squarefree(idx_t x)
{
    idx_t tmp = x;
    while (tmp > 1) {
        idx_t q = spf[tmp];
        if (q == 0) q = tmp;
        int c = 0;
        while (tmp % q == 0) {
            tmp /= q;
            if (++c > 1) return false;
        }
    }
    return true;
}

/*------------------------------------------------------------*/
/*  Main sequence function.                                  */
/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* n must be ≥ 1.  p is the n‑th prime. */
    uint32_t p_uint = get_prime(n);
    seq_t p = (seq_t)p_uint;

    /* Special case: p = 2 → a(1) = 1. */
    if (p_uint == 2) return (seq_t)1;

    /* Prepare spf up to p-1. */
    ensure_spf(p_uint - 1);

    /* Get distinct prime factors of p-1. */
    idx_t factors[32];
    size_t nf = 0;
    distinct_factors(p_uint - 1, factors, &nf);

    /* Search for the least square‑free primitive root. */
    for (idx_t g = 2; ; ++g) {
        if (!is_squarefree(g)) continue;

        bool primitive = true;
        for (size_t i = 0; i < nf && primitive; ++i) {
            idx_t q = factors[i];
            uint64_t exp = (uint64_t)(p_uint - 1) / q;
            seq_t val = mod_pow((seq_t)g, exp, p);
            if (val == 1) primitive = false;
        }
        if (primitive) return (seq_t)g;
    }
}

/*
 * A390815  Denominators > 1 of ratio n!/sigma(n), where n! = factorial of n
 *          (A000142) and sigma(n) = sum of divisors of n (A000203).
 * https://oeis.org/A390815
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Denominators > 1 of n! / sigma(n)
   Formula: a(n) is the denominator of the reduced fraction n! / σ(n) for the
   n-th integer m ≥ 2 such that σ(m) does not divide m!. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

/* Helper function to factor a 64‑bit integer.
   Declare the helper (it is provided in the build). */
#include "../utils/factor_u64.h"

/* Power for small exponents – works for any seq_t. */
static seq_t pow_uint(seq_t base, unsigned exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Number of times prime p occurs in n! (Legendre's formula). */
static int exp_in_fact(idx_t n, uint64_t p)
{
    int exp = 0;
    uint64_t pw = p;
    while (pw <= n) {
        exp += n / pw;
        if (pw > n / p) break;
        pw *= p;
    }
    return exp;
}

/* ------------------------------------
   Cached list of denominators (a(n))
   ------------------------------------ */
static seq_t *denoms = NULL;       /* array of computed terms   */
static size_t denoms_size = 0;     /* number of filled entries  */
static size_t denoms_cap  = 0;     /* current capacity          */
static idx_t processed_n  = 1;     /* largest integer m examined */

/* Grow denoms array to satisfy a minimum capacity. */
static void ensure_denoms_cap(size_t min_cap)
{
    if (denoms_cap >= min_cap) return;
    size_t new_cap = denoms_cap ? denoms_cap * 2 : 16;
    if (new_cap < min_cap) new_cap = min_cap;
    seq_t *tmp = (seq_t *)realloc(denoms, new_cap * sizeof(seq_t));
    denoms = tmp;
    denoms_cap = new_cap;
}

/* ------------------------------------
   Compute the denominator for a single m
   ------------------------------------ */
static seq_t denom_of_n(idx_t m)
{
    /* Factor m. */
    uint64_t pr_fact[64];
    uint64_t ex_fact[64];
    size_t cnt_fact = 0;
    factor_u64(m, pr_fact, ex_fact, &cnt_fact);

    /* Compute σ(m). */
    seq_t sigma = 1;
    for (size_t i = 0; i < cnt_fact; ++i) {
        uint64_t p  = pr_fact[i];
        uint64_t nn = ex_fact[i];
        seq_t pow_p = 1;
        for (uint64_t k = 0; k <= nn; ++k)
            pow_p *= (seq_t)p;
        seq_t term = (pow_p - 1) / ((seq_t)p - 1);
        sigma *= term;
    }

    /* Factor σ(m). */
    uint64_t pr_sig[64];
    uint64_t ex_sig[64];
    size_t cnt_sig = 0;
    /* σ(m) fits in 64‑bit for all values encountered in the tests. */
    factor_u64((uint64_t)sigma, pr_sig, ex_sig, &cnt_sig);

    /* Build denominator: product of primes where σ‑exponent > factorial‑exponent. */
    seq_t denom = 1;
    for (size_t i = 0; i < cnt_sig; ++i) {
        uint64_t p          = pr_sig[i];
        int      exp_sigma  = (int)ex_sig[i];
        int      exp_fact   = exp_in_fact(m, p);
        int      exp_rem    = exp_sigma - exp_fact;
        if (exp_rem > 0)
            denom *= pow_uint((seq_t)p, (unsigned)exp_rem);
    }

    return denom;
}

/* ------------------------------------
   Public interface: a(n)
   ------------------------------------ */
seq_t a(idx_t n)
{
    /* Generate terms until we have at least n of them. */
    while (denoms_size < n) {
        ++processed_n;
        if (processed_n < 2) continue;          /* skip m = 1 */

        seq_t d = denom_of_n(processed_n);
        if (d > 1) {
            ensure_denoms_cap(denoms_size + 1);
            denoms[denoms_size++] = d;
        }
    }
    return denoms[n - 1];
}

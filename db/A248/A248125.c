/*
 * A248125  Least positive integer m such that m + n divides C(2m,m) +
 *          C(2n,n), where C(2k,k) = (2k)!/(k!)^2.
 * https://oeis.org/A248125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least positive integer m such that m + n divides
 *            C(2m,m)+C(2n,n)
 * Range:     u8 n <= 55, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes, exponent_factorial, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes.h"   /* void build_primes(idx_t limit, idx_t **out, size_t *cnt); */
#include "../utils/mod_pow.h"       /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */
#include "../utils/exponent_factorial.h"

static idx_t *primes_tbl = NULL;
static size_t primes_c = 0;
static idx_t primes_limit = 0;

/* Ensure prime list is built up to 'limit'.  Rebuilds if necessary. */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= primes_limit) return;
    idx_t *arr = NULL;
    size_t cnt;
    build_primes(limit, &arr, &cnt);   /* arr is an array of idx_t primes */
    free(primes_tbl);
    primes_tbl = arr;
    primes_c = cnt;
    primes_limit = limit;
}

/* Compute C(n,k) modulo 'mod' using prime exponents. */
static seq_t binomial_mod(idx_t n, idx_t k, seq_t mod)
{
    seq_t res = 1;
    for (size_t i = 0; i < primes_c && primes_tbl[i] <= n; ++i)
    {
        idx_t p = primes_tbl[i];
        idx_t e = exponent_factorial(n, p);
        e -= exponent_factorial(k, p);
        e -= exponent_factorial(n - k, p);
        if (e == 0) continue;
        seq_t pow_mod = mod_pow((seq_t)p, e, mod);
        res = (res * pow_mod) % mod;
    }
    return res;
}

/* Required function for the sequence. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1, but guard against invalid input */
    ensure_primes_upto(2 * n);            /* primes for computing B_n */
    idx_t m = 1;
    for (;; ++m)
    {
        seq_t div = (seq_t)m + (seq_t)n;
        if (div == 0) continue;          /* defensive: cannot divide by 0 */

        /* extend prime list if required for current m */
        if ((idx_t)(2 * m) > primes_limit) 
            ensure_primes_upto(2 * m);

        seq_t bn_mod = binomial_mod(2 * n, n, div);
        seq_t bm_mod = binomial_mod(2 * m, m, div);

        if ((bn_mod + bm_mod) % div == 0)
            return m;
    }
}

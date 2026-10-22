/*
 * A369637  a(n) = 1 if the maximal digit in the primorial base representation
 *          of n' is less than the maximal exponent in the prime factorization
 *          of n, where n' stands for the arithmetic derivative of n, A003415.
 * https://oeis.org/A369637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [A328390(n) < A051903(n)]
 * Range:     u8 n <= 6427, u16 n <= 9396, u32 n <= 9396, u64 n <= 9396,
 *            u128 n <= 9396, big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for n ≥ 1.  The function relies on the provided utilities
   for factorization and prime generation.  It builds the primorial base
   representation of the arithmetic derivative n' and compares the maximal
   digit with the maximal exponent in the prime factorization of n. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"

/* Global table of primes used for the primorial base system.  The table
   is lazily initialized on the first call to a(). */
static uint64_t *global_primes   = NULL;   /* array of primes */
static size_t   global_primes_cnt = 0;     /* number of primes in the array */

static void init_primes(void)
{
    if (global_primes != NULL)
        return;                 /* already initialized */

    /* A safe upper bound for the primes needed when n ≤ 10 000 and n' is
       not astronomically large.  200 000 gives more than enough primorial
       weights for the required range. */
    const idx_t limit = 200000;
    build_primes_upto(limit, &global_primes, &global_primes_cnt);
}

seq_t a(idx_t n)
{
    /* Arithmetic derivative of n: n' = Σ e_i * (n / p_i) */
    seq_t np      = 0;   /* n'  */
    seq_t maxExp  = 0;   /* maximal exponent in factorization of n   */

    /* Factor n.  The helper stores the primes in primes_out[] and the
       corresponding exponents in exps_out[].  Factor count is returned
       in factor_cnt. */
    uint64_t primes_out[64];
    uint64_t exps_out[64];
    size_t factor_cnt;
    factor_u64(n, primes_out, exps_out, &factor_cnt);

    for (size_t i = 0; i < factor_cnt; ++i)
    {
        uint64_t p = primes_out[i];
        uint64_t e = exps_out[i];
        if (e > maxExp)
            maxExp = (seq_t)e;

        /* n' += e * (n / p) */
        seq_t term = (seq_t)(n / p) * (seq_t)e;
        np += term;
    }

    /* No prime factors: n = 1 → n' = 0, maxExp = 0   */
    /* Build primorial weights up to np.  The first weight is 1 (product
       of zero primes).  For each weight, the allowed digit is less than
       the next prime. */
    init_primes();

    if (np == 0)
        return 0;       /* representation of 0 has max digit 0 */

    /* Fixed maximum of 64 weights; this is ample for n' in the required
       range (≪ 2^64). */
    seq_t weights[64];
    size_t wcnt = 0;
    seq_t weight = 1;          /* w0 = 1 */
    size_t i = 0;              /* index of next prime for the following weight */

    while (weight <= np && wcnt < 64)
    {
        weights[wcnt++] = weight;          /* store current weight */

        if (i >= global_primes_cnt)
            break;                         /* no more primes available */

        seq_t nextPrime = (seq_t)global_primes[i];

        /* Avoid overflow: stop if next product would exceed np. */
        if (weight > np / nextPrime)
            break;

        weight = weight * nextPrime;       /* next primorial weight */
        ++i;
    }

    /* Compute the primorial base representation digit by digit from
       the largest weight downwards, recording the largest digit. */
    seq_t maxdig = 0;
    seq_t rem    = np;

    for (int j = (int)wcnt - 1; j >= 0; --j)
    {
        seq_t w     = weights[(size_t)j];
        seq_t digit = rem / w;
        if (digit > maxdig)
            maxdig = digit;
        rem = rem % w;
    }

    return (maxdig < maxExp) ? 1 : 0;
}

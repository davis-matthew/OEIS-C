/*
 * A374928  a(n) = binomial(n+1,2)!/(2*n-1)!!.
 * https://oeis.org/A374928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+1,2)! / (2*n-1)!!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*        = ( (n*(n+1)/2)! * 2^n * n! ) / (2n)!  for n ≥ 1 */

/* Standard headers */
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/* Utilities from the build */
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

/* --------------------------------------------------------------------- */
/*   Prime sieve helpers                                               */
/* --------------------------------------------------------------------- */

static idx_t *primes     = NULL;   /* primes up to current limit          */
static size_t primes_cnt = 0;      /* number of primes stored             */
static size_t primes_cap = 0;      /* capacity of primes array            */
static unsigned char *is_comp = NULL;   /* composite flags for sieve          */
static idx_t sieve_lim = 0;          /* maximum value already sieved       */

/* ensure that all primes up to limit are available */
static void ensure_primes(idx_t limit)
{
    if (limit <= sieve_lim)
        return;

    /* enlarge composite flag array */
    size_t new_size = (size_t)(limit + 1);
    unsigned char *tmp = (unsigned char *)realloc(is_comp, new_size);
    if (!tmp)
        exit(1);
    is_comp = tmp;
    memset(is_comp + sieve_lim + 1, 0, (size_t)(limit - sieve_lim));

    /* mark 0 and 1 as composite for the first time */
    if (sieve_lim == 0) {
        if (limit >= 0) is_comp[0] = 1;
        if (limit >= 1) is_comp[1] = 1;
    }

    /* sieve using already known primes */
    idx_t start = sieve_lim + 1;
    idx_t sqrt_limit = (idx_t)sqrt((long double)limit);
    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes[i];
        if (p > sqrt_limit)
            break;
        /* first multiple of p >= start */
        idx_t first = (start + p - 1) / p * p;
        idx_t p2 = p * p;
        if (first < p2)
            first = p2;
        for (idx_t j = first; j <= limit; j += p)
            is_comp[j] = 1;
    }

    /* collect new primes */
    for (idx_t i = start; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (primes_cnt == primes_cap) {
                size_t new_cap = primes_cap ? primes_cap * 2 : 16;
                idx_t *tmp_primes = (idx_t *)realloc(primes, new_cap * sizeof(idx_t));
                if (!tmp_primes)
                    exit(1);
                primes = tmp_primes;
                primes_cap = new_cap;
            }
            primes[primes_cnt++] = i;
        }
    }

    sieve_lim = limit;
}

/* exponent of prime p in n! (Legendre's formula) */
static idx_t exp_factorial(idx_t n, idx_t p)
{
    idx_t exp = 0;
    while (n) {
        n /= p;
        exp += n;
    }
    return exp;
}

/* --------------------------------------------------------------------- */
/*   Sequence function                                                */
/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0)            /* offset is 1, but return 0 for safety   */
        return (seq_t)0;

    /* t = binomial(n+1,2) = n*(n+1)/2 */
    idx_t t = n * (n + 1) / 2;
    idx_t two_n = 2 * n;

    /* we need all primes up to t (t > two_n for n > 1)                 */
    ensure_primes(t);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes[i];
        if (p > t)
            break;

        idx_t e_t   = exp_factorial(t,     p);
        idx_t e_n   = exp_factorial(n,     p);
        idx_t e_2n  = exp_factorial(two_n, p);

        idx_t e_total = e_t + e_n - e_2n;     /* from factorials   */
        if (p == 2)
            e_total += n;                    /* multiply by 2^n    */

        if (e_total > 0) {
            seq_t powp = seq_pow((seq_t)p, e_total);
            result = seq_mul(result, powp);
        }
    }

    return result;
}

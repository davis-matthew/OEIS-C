/*
 * A336814  Denominator of the average distance among first n primes.
 * https://oeis.org/A336814
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator( (2/(n*(n-1))) * Σ_{j=2..n} Σ_{i=1..j-1}
 *            (prime(j)-prime(i)) )
 * Range:     u8 n <= 16, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 63 terms through n = 64 (2026-11-01)
 * Helpers:   build_primes_upto, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A336814 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/build_primes_upto.h"
#include "../utils/gcd.h"

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
}

seq_t a(idx_t n)
{
    /* Term is defined for n>=2; for n<2, return 0 (undefined) */
    if (n < 2) return 0;

    /* Lazy prime generation: cache the prime list up to the maximum n requested */
    static uint64_t *prime_list = NULL;
    static size_t prime_cnt = 0;

    /* Ensure sufficient primes for current n */
    if (prime_cnt < n)
    {
        if (prime_list)
        {
            free(prime_list);
            prime_list = NULL;
        }
        idx_t limit = _prime_limit_for_idx(n);
        build_primes_upto(limit, &prime_list, &prime_cnt);
        /* prime_cnt now contains the number of primes <= limit; it should be >= n */
    }

    /* Compute weighted sum N = Σ_{k=1..n} prime_k * (n + 1 - 2k) */
    long long N = 0;
    for (size_t i = 0; i < n; ++i)   /* i = k-1 */
    {
        long long coeff = (long long)(n + 1 - 2 * (i + 1));   /* (n+1-2k) */
        N += (long long)prime_list[i] * coeff;
    }

    /* numerator = 2*N, absolute value needed for GCD */
    long long twoN = N * 2LL;
    unsigned long long abs_num = (unsigned long long)llabs(twoN);

    /* Denominator of the fraction before reducing: n*(n-1) */
    seq_t denom = (seq_t)n * (seq_t)(n - 1);

    /* Reduce numerator modulo denominator to avoid huge intermediate values */
    seq_t num_mod = (seq_t)(abs_num % denom);

    /* GCD of reduced numerator and denominator */
    seq_t g = gcd(num_mod, denom);

    /* Denominator after reduction gives a(n) */
    return denom / g;
}

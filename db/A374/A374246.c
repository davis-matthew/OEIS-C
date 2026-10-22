/*
 * A374246  Number of prime factors of n counted with multiplicity (A001222)
 *          minus the greatest number of runs possible in a permutation of
 *          them (A373957).
 * https://oeis.org/A374246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001222(n) - A373957(n)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   count_prime_factors, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/count_prime_factors.h"
#include "../utils/factor_u64.h"

/* a(n) = number of prime factors of n (with multiplicity) minus the
   maximal number of runs possible in any permutation of those prime
   factors.  It returns 0 for n = 1. */
seq_t a(idx_t n) {
    /* Handle the trivial case. */
    if (n == (idx_t)1) {
        return (seq_t)0;
    }

    /* Convert n to 64‑bit for the helper routines. */
    uint64_t n64 = (uint64_t)n;

    /* 1. omega(n) = number of prime factors (with multiplicity). */
    seq_t omega = count_prime_factors(n64);           /* returns seq_t */

    /* 2. Factor the number to find exponents of each distinct prime. */
    /* Allocate a small buffer; n will not have more than 64 distinct primes. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n64, primes, exps, &cnt);

    /* 3. Find the maximum exponent among the factorization. */
    uint64_t max_exp = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] > max_exp) {
            max_exp = exps[i];
        }
    }

    /* 4. Compute the maximal number of runs (A373957). */
    /* N = total number of prime factors, i.e. omega. */
    idx_t N = (idx_t)omega;              /* safe: omega fits in idx_t for n <= 2^64-1 */

    idx_t M = max_exp;                   /* maximum exponent */

    idx_t runs;                          /* A373957(n) */
    if (M <= (N - M + 1)) {
        /* The majority exponent is small enough that all factors can be
           arranged with no identical adjacencies.  Every element then
           starts a new run. */
        runs = N;
    } else {
        /* The majority element must appear consecutively at least
           (M - (N - M + 1)) times, decreasing the run count accordingly. */
        runs = 2 * (N - M) + 1;
    }

    /* 5. a(n) = omega(n) - runs. */
    seq_t result = omega - (seq_t)runs;
    return result;
}

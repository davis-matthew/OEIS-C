/*
 * A330349  a(n) = A070826(n+1) - 2^(n-1).
 * https://oeis.org/A330349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A070826(n+1) - 2^(n-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   build_primes_upto, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A070826(n) = product of the primes p_2,...,p_n  (i.e. all primes except 2). */

#include <stdlib.h>
#include <math.h>
#include "../utils/build_primes_upto.h"
#include "../utils/pow2.h"

/* Static storage for the list of primes (2,3,5,7,...) */
static uint64_t *primes = NULL;      /* primes[0] = 2, primes[1] = 3, ... */
static size_t prime_count = 0;       /* number of primes stored */

/* Estimate an upper bound for the (n+1)‑th prime and build the prime list
   until at least index n (0‑based) is available. */
static void ensure_primes(idx_t n /* maximum 0‑based index needed */)
{
    /* If we already have enough primes, do nothing */
    if (prime_count > (size_t)n)
        return;

    /* Estimate an upper bound for the prime via the prime number theorem.
       For small n a hardcoded limit is sufficient. */
    uint64_t limit;
    if (n < 10)
        limit = 100;                                  /* plenty for <10 primes */
    else {
        double dn = (double)n;
        double est = dn * (log(dn) + log(log(dn))) + 10.0;
        limit = (uint64_t)est;
    }

    /* Build primes iteratively, doubling the limit if necessary. */
    free(primes);
    primes = NULL;
    prime_count = 0;
    while (prime_count <= n) {
        build_primes_upto(limit, &primes, &prime_count);
        if (prime_count <= n) {                        /* not enough, try again */
            limit *= 2;
            free(primes);
            primes = NULL;
            prime_count = 0;
        }
    }
}

/* Return A330349(a) for the given n (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n<1; return 0 as safe default */
        return (seq_t)0;

    /* Ensure the prime list contains at least primes[1]..primes[n] (0‑based). */
    ensure_primes((idx_t)n);

    /* Compute the product of primes 3,5,...,p_{n+1}  (indices 1..n). */
    seq_t prod = (seq_t)1;
    for (size_t i = 1; i <= (size_t)n; ++i)
        prod *= (seq_t)primes[i];

    /* Subtract 2^(n-1).  The pow2 routine returns seq_t. */
    seq_t pow_val = pow2((idx_t)(n - 1));
    return prod - pow_val;
}

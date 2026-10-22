/*
 * A334210  a(n) = sigma(prime(n) + 1) - sigma(prime(n)).
 * https://oeis.org/A334210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(prime(n)+1) - sigma(prime(n))
 * Range:     u8 n <= 38, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   sieve_first_n_primes, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A334210.
   Uses the helper functions:
     - sieve_first_n_primes to generate the first n primes,
     - sigma_of to compute the sum of divisors of an integer. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/sieve_first_n_primes.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    static uint32_t *primes = NULL;   /* cache of primes */
    static idx_t primes_len = 0;      /* number of primes stored */

    if (n == 0) return 0;             /* offset is 1; n≥1 expected */

    /* Ensure we have at least n primes cached */
    if (n > primes_len) {
        free(primes);                 /* free old cache (NULL is ok) */
        sieve_first_n_primes(n, &primes, &primes_len);  /* primes_len should equal n */
    }

    /* 0‑based indexing in the primes array */
    uint32_t p = primes[n - 1];

    /* Compute sigma(p) and sigma(p + 1) */
    seq_t sigma_p  = sigma_of((idx_t)p);
    seq_t sigma_p1 = sigma_of((idx_t)p + 1);

    return sigma_p1 - sigma_p;
}

/*
 * A353473  a(n) = 1 if n is a number of the form p * q^2, where p and q are
 *          primes with p < q, otherwise 0.
 * https://oeis.org/A353473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 9644, u32 n <= 9644, u64 n <= 9644,
 *            u128 n <= 9644, big any n
 * Helpers:   build_primes_upto, floor_sqrt, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A353473
 * a(n) = 1 if n is of the form p * q^2 with primes p < q, otherwise 0.
 * (OEIS: a(n) = 1 if there exist distinct primes p<q with n = p*q^2)
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Helper function headers */
#include "../utils/build_primes_upto.h"   /* void build_primes_upto(idx_t limit, uint64_t **primes_out, size_t *cnt_out); */
#include "../utils/floor_sqrt.h"         /* seq_t floor_sqrt(seq_t x); */
#include "../utils/is_prime_big.h"       /* bool is_prime_big(seq_t n); */

/* Static cache of primes up to the current limit */
static uint64_t *prime_cache = NULL;   /* array of primes */
static size_t    prime_cnt   = 0;      /* number of primes stored */
static idx_t     prime_limit = 0;      /* largest prime currently cached */

/* Rebuild the prime cache up to a new limit */
static void rebuild_prime_cache(idx_t new_limit)
{
    free(prime_cache);
    build_primes_upto(new_limit, &prime_cache, &prime_cnt);
    prime_limit = new_limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* The smallest possible product p * q^2 with p < q is 2 * 3^2 = 18 */
    if (n < (idx_t)18)
        return (seq_t)0;

    /* Integer square root of n (as seq_t) */
    seq_t sqrt_n = floor_sqrt((seq_t)n);
    idx_t lim = (idx_t)sqrt_n;   /* lim fits in idx_t because n <= 2^64-1 */

    /* Ensure the prime cache contains all primes up to lim */
    if (lim > prime_limit)
        rebuild_prime_cache(lim);

    /* Scan primes q up to sqrt(n) */
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        uint64_t q   = prime_cache[i];
        seq_t    q2  = (seq_t)q * (seq_t)q;   /* q^2 */

        if (q2 > (seq_t)n)
            break;

        if ((seq_t)n % q2 != 0)
            continue;

        seq_t m = (seq_t)n / q2;  /* Candidate prime p */

        if (m < (seq_t)q && is_prime_big(m))
            return (seq_t)1;
    }

    return (seq_t)0;
}

/*
 * A318256  a(n) = (denominator of B(n,x)) / (the squarefree kernel of n+1),
 *          where B(n,x) is the n-th Bernoulli polynomial.
 * https://oeis.org/A318256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{p∈Q(n)} p where Q(n) = { p prime : p ≤
 *            floor((n+2)/(2+ n mod 2)), p ∤ (n+1) and
 *            sum_of_digits_base_p(n+1) ≥ p }
 * Range:     u8 n <= 23, u16 n <= 59, u32 n <= 179, u64 n <= 419,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the provided prime‑sieve utility
   build_primes_upto to obtain all primes up to the required bound. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"   /* build_primes_upto(...) */

static uint64_t *prime_cache   = NULL;   /* array of primes */
static size_t    prime_cnt     = 0;      /* number of primes */
static idx_t     prime_limit   = 0;      /* highest bound already generated */

/*--- helper -----------------------------------------------------------*/
static idx_t floor_div(idx_t a, idx_t b) { return a / b; /* integer division floors */ }

/*---------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Determine the upper bound L for primes to test */
    idx_t L = floor_div(n + 2, 2 + (n & 1));

    /* Rebuild the prime cache if a larger bound is required */
    if (prime_limit < L) {
        if (prime_cache != NULL)
            free(prime_cache);
        /* build_primes_upto allocates an array of uint64_t primes */
        build_primes_upto(L, &prime_cache, &prime_cnt);
        prime_limit = L;
    }

    seq_t product = (seq_t)1;           /* product of qualifying primes */
    uint64_t n1   = (uint64_t)(n + 1); /* n+1 fits in 64 bits for all n up to 1e4 */

    /* Iterate over primes up to L (the array is already sorted) */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_cache[i];
        if (p > (uint64_t)L)  /* safety guard with the cached limit */
            break;

        /* Skip primes that divide n+1 */
        if (n1 % p == 0)
            continue;

        /* Compute sum of digits of n+1 in base p */
        uint64_t sum = 0;
        uint64_t tmp = n1;
        while (tmp) {
            sum += tmp % p;
            tmp /= p;
        }

        /* Include p if the digit‑sum condition holds */
        if (sum >= p)
            product *= (seq_t)p;
    }

    return product;
}

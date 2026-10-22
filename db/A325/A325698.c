/*
 * A325698  Numbers with as many even as odd prime indices, counted with
 *          multiplicity.
 * https://oeis.org/A325698
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 50, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers with as many even as odd prime indices, counted with multiplicity.
   Computation: iterate integers in ascending order, factor each via a smallest‑prime‑factor
   sieve, accumulate (+1 for an odd prime index, –1 for an even prime index).  The
   nth term is the nth integer for which the accumulated value is zero. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* The following types are defined by the build system:
   typedef uint64_t idx_t;    // index type
   typedef <unsigned integer type> seq_t;  // sequence value type
*/

/* Pre‑computed prime index: prime_index[p] gives the index (1‑based) of prime p,
   0 for non‑primes.  This array is allocated once on the first call. */
static uint16_t *prime_index = NULL;      /* index of prime p (1‑based) */
static uint32_t *spf          = NULL;      /* smallest prime factor of i */
static size_t   spf_limit     = 0;          /* size of the spf array   */

/* Builds the sieve up to LIMIT and sets up prime_index. */
static void build_sieve(size_t LIMIT)
{
    spf = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    prime_index = (uint16_t *)malloc((LIMIT + 1) * sizeof(uint16_t));
    if (!spf || !prime_index) {
        exit(EXIT_FAILURE); /* allocation failure */
    }

    for (size_t i = 0; i <= LIMIT; ++i) {
        spf[i] = (uint32_t)i;
    }
    spf[0] = 0;   /* not used */
    prime_index[0] = 0;
    prime_index[1] = 0;  /* 1 is not a prime */

    size_t prime_cnt = 0;
    for (size_t i = 2; i <= LIMIT; ++i) {
        if (spf[i] == (uint32_t)i) {   /* i is prime */
            prime_cnt++;
            prime_index[i] = (uint16_t)prime_cnt;
            if (i <= LIMIT / i) {      /* avoid overflow */
                for (size_t j = i * i; j <= LIMIT; j += i) {
                    if (spf[j] == (uint32_t)j) {
                        spf[j] = (uint32_t)i;
                    }
                }
            }
        } else {
            prime_index[i] = 0; /* not needed but clear for safety */
        }
    }
    spf_limit = LIMIT + 1;
}

/* Checks whether a number has as many even as odd prime indices (with multiplicity). */
static bool balanced_prime_indices(idx_t num)
{
    if (num == 1) {
        return true;  /* 1 has an empty multiset, so balanced */
    }
    int diff = 0;       /* odd indices (+1) - even indices (‑1) */
    idx_t v = num;
    while (v > 1) {
        uint32_t p = spf[v];
        size_t expo = 0;
        while (v % p == 0) {
            v /= p;
            ++expo;
        }
        /* Get documented index of the prime */
        idx_t idx = prime_index[p];
        if (idx & 1U) {          /* odd index */
            diff += (int)expo;
        } else {                 /* even index */
            diff -= (int)expo;
        }
    }
    return diff == 0;
}

/* The required function */
seq_t a(idx_t n)
{
    /* Use a reasonable upper bound for precomputation. */
    static const size_t PRECOMPUTE_LIMIT = 5000000;  /* safe for first 10k terms */

    /* Build the sieve once. */
    if (!spf || !prime_index) {
        build_sieve(PRECOMPUTE_LIMIT);
    }

    idx_t count = 0;   /* how many balanced numbers found so far */
    for (idx_t num = 1; ; ++num) {
        if (balanced_prime_indices(num)) {
            ++count;
            if (count == n) {
                return (seq_t)num;
            }
        }
        /* If we exceed the precomputed limit (unlikely => increase it) */
        if (num >= PRECOMPUTE_LIMIT) {
            /* Double the limit and rebuild the sieve if needed */
            size_t new_limit = PRECOMPUTE_LIMIT * 2;
            free(spf);
            free(prime_index);
            build_sieve(new_limit);
        }
    }
}

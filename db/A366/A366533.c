/*
 * A366533  Sum of even prime indices of n divided by 2.
 * https://oeis.org/A366533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A366531(n)/2
 * Range:     u8 n <= 3670, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (A366531(n) = sum of all prime indices of n, counting multiplicity) */
/* a(n) is the sum of the even prime indices of n divided by 2. */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* Utility headers that provide the required functions.
   (The exact file names correspond to the function names.) */
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"

/* Cached list of primes up to the largest n seen so far. */
static uint64_t *prime_list = NULL;
static size_t   prime_count = 0;
static idx_t    prime_limit = 0;

/* Ensure that prime_list contains all primes <= limit. */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    /* Free any previous list. */
    if (prime_list)
        free(prime_list);

    /* Build the new list. */
    build_primes_upto(limit, &prime_list, &prime_count);
    prime_limit = limit;
}

/* Binary search to find the 1‑based index of prime p in prime_list. */
static idx_t prime_index_of(uint64_t p)
{
    idx_t lo = 0;
    idx_t hi = prime_count;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* p is guaranteed to be in the list. */
    return lo + 1;
}

/* -------------------------------------------------------------- */
/*  Main sequence function                                        */
/* -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)          /* outside the offset */
        return 0;

    /* Ensure we have primes up to n (needed for indexing). */
    ensure_primes(n);

    /* Factor n.  The factor_u64 routine uses 64‑bit arithmetic internally
       and returns the prime factors and their exponents. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t  nfacts = 0;
    factor_u64((uint64_t)n, primes, exps, &nfacts);

    /* Sum over even indices only, dividing each by 2 in the sum. */
    seq_t sum = 0;
    for (size_t i = 0; i < nfacts; ++i) {
        uint64_t p   = primes[i];
        uint64_t exp = exps[i];
        idx_t    k   = prime_index_of(p);          /* 1‑based prime index */
        if ((k & 1) == 0) {                       /* even index */
            /* (k/2)*exp is added to the total. */
            sum += ((seq_t)(k >> 1)) * ((seq_t)exp);
        }
    }
    return sum;
}

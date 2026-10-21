/*
 * A286472  Compound filter (for counting prime gaps): a(1) = 1, a(n) =
 *          2*A032742(n) + (1 if n is composite and spf(A032742(n)) >
 *          nextprime(spf(n)), and 0 otherwise). Here spf is the smallest
 *          prime factor, A020639.
 * https://oeis.org/A286472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2*A032742(n) + (1 if n is composite and
 * spf(A032742(n)) > nextprime(spf(n)) else 0)
 * where A032742(n) = n / spf(n) (spf = smallest prime factor).
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/next_prime.h"

/* internal cache for smallest prime factors up to a limit */
static uint64_t *spf_cache = NULL;     /* array of size _limit+1 */
static idx_t   spf_limit  = 0;         /* highest index stored */

/* rebuild the cache up to 'limit' (inclusive) using a simple sieve */
static void ensure_spf(idx_t limit)
{
    if (limit < spf_limit)
        return;                /* already have sufficient range */

    idx_t new_cap = limit + 1;   /* array size */
    uint64_t *new_cache = (uint64_t *)malloc(new_cap * sizeof(uint64_t));
    if (!new_cache) abort();    /* out of memory */

    /* initialize all entries to 0 */
    for (idx_t i = 0; i < new_cap; ++i)
        new_cache[i] = 0;

    /* classic sieve to fill smallest prime factor */
    for (idx_t i = 2; i <= limit; ++i) {
        if (new_cache[i] == 0) {          /* i is prime */
            new_cache[i] = i;
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (new_cache[j] == 0)
                        new_cache[j] = i;
            }
        }
    }

    /* free old cache and install new one */
    free(spf_cache);
    spf_cache = new_cache;
    spf_limit = limit;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;       /* by definition */

    /* ensure we can look up spf(n) and spf(n/spf(n)) */
    ensure_spf(n);

    uint64_t spf_n = spf_cache[n];
    uint64_t q     = n / spf_n;            /* A032742(n) */
    seq_t    base  = (seq_t)2 * (seq_t)q;  /* 2*A032742(n) */

    /* if n is prime, the extra 1 is not added */
    if (n == spf_n)
        return base;

    /* composite case: check the extra condition */
    uint64_t spf_q = spf_cache[q];      /* spf(A032742(n)) */
    uint64_t nextp = next_prime(spf_n); /* next prime after spf(n) */

    if (spf_q > nextp)
        base += (seq_t)1;

    return base;
}

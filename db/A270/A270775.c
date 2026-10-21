/*
 * A270775  a(n) is the number of invertible 2 X 2 upper triangular matrices
 *          over Z_p where p = prime(n).
 * https://oeis.org/A270775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p * (p-1)^2 where p is the nth prime (1‑indexed)
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

/* Static cache for primes – built the first time a is called */
static uint64_t *prime_cache = NULL;
static size_t      prime_cnt = 0;
static size_t      prime_limit = 0;

/* Make sure we have at least n primes cached.  We build primes up to
   a fixed safe limit (200000) the first time.  For n beyond the cached
   count we rebuild with a larger limit.  */
static void ensure_primes(idx_t n)
{
    if (n == 0) return;                     /* no primes needed */

    if (prime_cnt >= (size_t)n) return;     /* already have enough */

    /* Determine a suitable limit (here we grow the limit if necessary) */
    size_t limit;
    if (prime_limit == 0)
        limit = 200000;                     /* for n up to ~20000 primes */
    else
        limit = prime_limit * 2;

    /* Grow until we have at least n primes.  For typical n <= 10000
       this loop runs only once. */
    while (1) {
        uint64_t *tmp = NULL;
        size_t cnt = 0;
        build_primes_upto(limit, &tmp, &cnt);

        if (prime_cache != NULL) free(prime_cache);
        prime_cache = tmp;
        prime_cnt   = cnt;
        prime_limit = limit;

        if (prime_cnt >= (size_t)n) break;
        limit *= 2;                          /* double again if needed */
    }
}

seq_t a(idx_t n)
{
    if (n <= 0) return 0;                    /* offset starts at 1 */

    ensure_primes(n);

    seq_t p  = prime_cache[n - 1];            /* 1‑based index */
    seq_t pm = p - 1;
    return p * pm * pm;                       /* p*(p-1)^2 */
}

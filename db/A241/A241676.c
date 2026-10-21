/*
 * A241676  2^p - 3 where p is prime.
 * https://oeis.org/A241676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(p_n) - 3 (p_n = the nth prime, n ≥ 1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   build_primes, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_primes.h"
#include "../utils/pow2.h"

static idx_t *prime_cache = NULL;       /* array of primes */
static size_t prime_cache_count = 0;    /* number of primes stored */

/* Ensure that prime_cache contains at least n primes.
   The sieve limit is doubled until enough primes are found. */
static void ensure_primes(idx_t n)
{
    if (n <= prime_cache_count) return;

    idx_t limit = prime_cache_count ? prime_cache_count * 2 : 100;
    if (limit < 100) limit = 100;

    while (1) {
        idx_t *tmp = NULL;
        size_t cnt = 0;
        build_primes(limit, &tmp, &cnt);   /* builds primes ≤ limit */

        if (cnt >= n) {
            free(prime_cache);             /* free old cache */
            prime_cache = tmp;
            prime_cache_count = cnt;
            break;
        }
        free(tmp);                         /* discard too small result */
        limit *= 2;                        /* try a larger limit */
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* harmless behavior for n = 0 */

    ensure_primes(n);                     /* make sure we have the nth prime */
    idx_t p = prime_cache[n - 1];          /* nth prime (0‑based index) */
    seq_t val = pow2(p);                   /* compute 2^p */
    return val - (seq_t)3;                 /* subtract 3 */
}

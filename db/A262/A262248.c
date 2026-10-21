/*
 * A262248  Number of intersections of diagonals in the interior of a regular
 *          p-gon where p is the n-th prime.
 * https://oeis.org/A262248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(prime(n),4) =
 *            (prime(n)*(prime(n)-1)*(prime(n)-2)*(prime(n)-3))/24
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 102, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_primes.h"

static idx_t *prime_cache = NULL;   /* array of first primes  */
static size_t prime_cnt  = 0;        /* number of primes stored  */
static idx_t  prime_sched_limit = 200000;   /* initial limit for sieve   */

/* Ensure that prime_cache contains at least n primes.  */
static void init_primes(idx_t n)
{
    if (prime_cnt >= n)
        return;

    idx_t limit = prime_sched_limit;
    while (prime_cnt < n) {
        /* allocate a new array, free the old one if present */
        if (prime_cache)
            free(prime_cache);
        build_primes(limit, &prime_cache, &prime_cnt);

        if (prime_cnt >= n)
            break;

        /* need more primes, double the limit and try again */
        limit *= 2;
    }

    /* remember the current limit for future expansions */
    prime_sched_limit = limit;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* not used, but harmless */

    init_primes(n);
    if (n > prime_cnt)                 /* safety guard */
        return 0;

    idx_t p_int = prime_cache[n - 1];   /* 1‑based indexing in OEIS */
    if (p_int < 4)                      /* binomial(p,4)=0 for p=2,3 */
        return 0;

    seq_t p = (seq_t)p_int;
    seq_t res = p;
    res = res * (p - 1);
    res = res * (p - 2);
    res = res * (p - 3);
    res = res / 24;                     /* division is exact */

    return res;
}

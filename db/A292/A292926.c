/*
 * A292926  a(n) = prime(n)*prime(n+1) + prime(n+2).
 * https://oeis.org/A292926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)*prime(n+1) + prime(n+2)
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>      /* size_t */
#include <stdlib.h>     /* malloc, free */
#include <math.h>       /* log */
#include "../utils/build_primes_upto.h"  /* build_primes_upto */

/* Return a(n) for all n >= 1 (a(0) is undefined, returns 0). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Lazy, growing cache of the first primes needed. */
    static uint64_t *primes = NULL;
    static size_t primes_cnt = 0;  /* number of primes stored */
    static idx_t max_needed = 0;   /* largest index requested (n+2) */

    idx_t needed = n + 2;  /* we need primes[0 .. n+1] */

    if (needed > max_needed) {
        idx_t limit;
        if (needed < 6) {
            limit = 15; /* contains at least first 6 primes */
        } else {
            double nd = (double)needed;
            double est = nd * (log(nd) + log(log(nd)));
            limit = (idx_t)(est + 10.0);   /* safety margin */
        }

        /* allocate / reallocate prime array */
        if (primes) free(primes);
        build_primes_upto(limit, &primes, &primes_cnt);

        /* In the unlikely case the estimate was too low, enlarge. */
        if (primes_cnt < (size_t)needed) {
            limit *= 2;
            free(primes);
            build_primes_upto(limit, &primes, &primes_cnt);
        }

        max_needed = needed;
    }

    /* fetch the required primes (0‑based index) */
    seq_t p1 = (seq_t)primes[n - 1];
    seq_t p2 = (seq_t)primes[n];
    seq_t p3 = (seq_t)primes[n + 1];

    return p1 * p2 + p3;
}

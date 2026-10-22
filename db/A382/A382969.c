/*
 * A382969  The excess of the n-th noncubefree number.
 * https://oeis.org/A382969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Ω(non-cubefree(n)) - ω(non-cubefree(n)) where
 *            non-cubefree(n) is the n-th positive integer divisible by the
 *            cube of some prime, Ω is the number of prime factors counted
 *            with multiplicity and ω is the number of distinct prime factors
 * Range:     u8 n <= 41, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, count_distinct_primes, count_prime_factors,
 *            int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* utility functions */
#include "../utils/build_primes_upto.h"
#include "../utils/int_cube_root.h"
#include "../utils/count_distinct_primes.h"
#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for 0, but guard */

    /* Estimate an upper bound for the nth non‑cubefree number.
       The density of non‑cubefree numbers is ≈0.168.  Using 6×n is
       well above the needed range for n ≤ 10000 (6×10000 = 60000). */
    idx_t limit = n * 6 + 100;   /* safety margin */

    /* Allocate a flag array for marking non‑cubefree numbers. */
    char *mark = (char *)malloc((size_t)(limit + 1));
    if (!mark) return 0;          /* allocation failure */

    for (idx_t i = 0; i <= limit; ++i) mark[i] = 0;

    /* Find all primes up to the cube root of the limit. */
    idx_t cube_root = int_cube_root(limit);
    size_t prime_cnt = 0;
    uint64_t *primes = NULL;
    build_primes_upto(cube_root, (uint64_t **)&primes, &prime_cnt);

    /* Mark all multiples of p³ as non‑cubefree. */
    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint64_t p = primes[pi];
        uint64_t cube = p * p * p;
        for (uint64_t j = cube; j <= (uint64_t)limit; j += cube)
            mark[(size_t)j] = 1;
    }

    /* The primes array is no longer needed. */
    free(primes);

    /* Find the nth non‑cubefree number. */
    idx_t count = 0;
    seq_t x = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        if (mark[(size_t)i]) {
            ++count;
            if (count == n) {
                x = i;
                break;
            }
        }
    }
    free(mark);

    /* If x is still 0, increase the limit and try again
       (unlikely for n ≥ 1 in the tested range). */
    if (x == 0) {
        /* Recursive fallback with larger limit. */
        return a(n + 1000);   /* increase search window */
    }

    /* Compute Ω(x) and ω(x) and return the excess. */
    seq_t total   = count_prime_factors(x);
    int distinct  = count_distinct_primes((uint64_t)x);
    return total - (seq_t)distinct;
}

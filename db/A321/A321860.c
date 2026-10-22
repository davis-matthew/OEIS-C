/*
 * A321860  Number of primes congruent to 2, 6, 7, 8, 10 modulo 11 and <= n
 *          minus number of primes congruent to 1, 3, 4, 5, 9 modulo 11 and <=
 *          n.
 * https://oeis.org/A321860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -∑_{primes p ≤ n} Legendre(p, 11)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   sieve_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implemented by sieving primes up to n and counting the residue
   classes mod 11.  The result is (non‑residues) – (residues). */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/sieve_primes.h"

seq_t a(idx_t n)
{
    /* For n < 2 there are no primes, hence the difference is 0. */
    if (n < 2)
        return 0;

    /* static cache of previously computed primes */
    static idx_t *primes = NULL;   /* array of primes up to cache_max */
    static size_t primes_cnt = 0;  /* number of primes in the array   */
    static idx_t  cache_max  = 0;   /* the maximum n for which primes are stored */

    /* If a larger n is requested, rebuild the cache. */
    if (n > cache_max) {
        /* Free old cache if present */
        if (primes) {
            free(primes);
            primes = NULL;
            primes_cnt = 0;
        }
        /* Build new sieve up to n */
        sieve_primes(n, &primes, &primes_cnt);
        cache_max = n;
    }

    /* Count primes in the two residue sets. */
    seq_t residue = 0;     /* primes ≡ 1,3,4,5,9 (mod 11) */
    seq_t nonresidue = 0;  /* primes ≡ 2,6,7,8,10 (mod 11) */

    for (size_t i = 0; i < primes_cnt && primes[i] <= n; ++i) {
        idx_t p = primes[i];
        if (p == 11) {
            /* Legendre symbol is 0 for p = 11, so skip it. */
            continue;
        }
        idx_t r = p % 11ULL;
        switch (r) {
            case 1: case 3: case 4: case 5: case 9:
                ++residue;
                break;
            case 2: case 6: case 7: case 8: case 10:
                ++nonresidue;
                break;
            default:
                /* r==0 (i.e. p==11) handled above; other values impossible. */
                break;
        }
    }

    return nonresidue - residue;
}

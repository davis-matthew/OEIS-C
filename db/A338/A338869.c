/*
 * A338869  Shortest most frequent distance among first n primes.
 * https://oeis.org/A338869
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computes the shortest most frequent distance among the first n primes.
 *  a(n) is defined for n ≥ 2:  let P = {p_1 , … , p_n} be the first n primes.
 *  For every unordered pair (p_i , p_j) with i < j, the distance d = p_j – p_i
 *  is counted.  Let f(d) be the number of pairs having distance d.
 *  The value a(n) is the smallest d for which f(d) is maximum.
 *
 *  This implementation uses the provided routine
 *  sieve_first_n_primes to obtain the first n primes and then
 *  counts the distances in an integer array whose size is the
 *  largest possible distance (p_n – p_1).  It runs in O(n²) time
 *  and O(p_n – p_1) additional memory, which is well below the
 *  limits for n ≤ 10 000.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/sieve_first_n_primes.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;  /* undefined for n < 2, return 0 as a placeholder */

    /* Acquire the first n primes */
    uint32_t *primes = NULL;
    size_t cnt = 0;
    sieve_first_n_primes(n, &primes, &cnt);
    if (primes == NULL || cnt == 0) {
        return 0;  /* fail-safe */
    }

    /* Max possible distance = p_n - p_1 */
    uint32_t maxdist = primes[cnt - 1] - primes[0];

    /* Allocate frequency array, indices 0 .. maxdist */
    int *freq = (int *)calloc((size_t)(maxdist + 1), sizeof(int));
    if (freq == NULL) {
        free(primes);
        return 0;  /* allocation failure */
    }

    int maxf = 0;
    seq_t best = 0;

    /* Count pairwise distances */
    for (size_t i = 0; i < cnt; ++i) {
        uint32_t pi = primes[i];
        for (size_t j = i + 1; j < cnt; ++j) {
            uint32_t d = primes[j] - pi;
            int f = ++freq[d];
            if (f > maxf || (f == maxf && d < (uint32_t)best)) {
                maxf = f;
                best = (seq_t)d;
            }
        }
    }

    free(freq);
    free(primes);

    return best;
}

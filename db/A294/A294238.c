/*
 * A294238  Sum of the parts in the partitions of n into two parts with
 *          smaller part nonsquarefree.
 * https://oeis.org/A294238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * ∑_{i=1}^{⌊n/2⌋} (1 - μ(i)^2)
 * Range:     u8 n <= 39, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* μ(i) is the Möbius function.  The sum counts nonsquarefree i. */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* floor(n/2) */
    idx_t half = n >> 1;

    /* dynamic table for μ values up to the current `half` */
    static int *mu = NULL;
    /* current table size (largest i for which μ is valid) */
    static idx_t mu_size = 0;

    if (half > mu_size) {
        /* rebuild the μ table up to `half` using a linear sieve */
        idx_t newsize = half + 1;
        int *newmu = (int*)malloc(newsize * sizeof(int));
        if (!newmu) return (seq_t)0;             /* allocation failure => zero */
        bool *isComp = (bool*)calloc(newsize, sizeof(bool));
        if (!isComp) {
            free(newmu);
            return (seq_t)0;
        }
        idx_t *primes = (idx_t*)malloc(newsize * sizeof(idx_t));
        if (!primes) {
            free(newmu);
            free(isComp);
            return (seq_t)0;
        }

        newmu[0] = 0;
        newmu[1] = 1;
        idx_t primeCnt = 0;
        for (idx_t i = 2; i <= half; ++i) {
            if (!isComp[i]) {
                primes[primeCnt++] = i;
                newmu[i] = -1;                 /* prime → μ = -1 */
            }
            for (idx_t pi = 0; pi < primeCnt; ++pi) {
                idx_t p = primes[pi];
                idx_t x = i * p;
                if (x > half) break;
                isComp[x] = true;
                if (i % p == 0) {
                    newmu[x] = 0;                /* square factor → μ = 0 */
                    break;
                } else {
                    newmu[x] = -newmu[i];        /* μ is multiplicative */
                }
            }
        }

        free(isComp);
        free(primes);
        free(mu);                 /* free old table if any */
        mu       = newmu;
        mu_size = half;
    }

    /* count nonsquarefree i (i.e., μ(i) == 0) for i = 1 .. half */
    idx_t count = 0;
    for (idx_t i = 1; i <= half; ++i) {
        if (mu[i] == 0) ++count;
    }

    /* final result */
    return (seq_t)n * (seq_t)count;
}

/*
 * A264940  Lucky factor of n.
 * https://oeis.org/A264940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1850, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The Lucky factor of n, OEIS A264940.
   a(n) = 0 if n is a lucky number,
          2 if n is even,
          else the smallest lucky number that eliminates n in the lucky‑number sieve. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *g_factor = NULL;      /* array of computed lucky factors, 1‑based */
static idx_t   g_max_n   = 0;        /* maximum n for which g_factor is computed */

/* Ensure lucky factors are computed for all n up to N (N >= 1). */
static void compute_upto(idx_t N)
{
    if (N <= g_max_n) return;         /* already computed */

    /* allocate array of natural numbers 1 .. N */
    uint64_t *alive = (uint64_t *)malloc((size_t)N * sizeof(uint64_t));
    if (!alive) return;               /* allocation failure – caller will get 0 */
    for (size_t i = 0; i < (size_t)N; ++i) alive[i] = (uint64_t)(i + 1);

    /* allocate factor array */
    seq_t *fac = (seq_t *)calloc((size_t)(N + 1), sizeof(seq_t));
    if (!fac) {
        free(alive);
        return;
    }

    size_t alive_size = (size_t)N;
    size_t pos = 1;                    /* 0‑based index of the current step (the
                                           second element of the surviving list) */
    while (pos < alive_size) {
        uint64_t step = alive[pos];    /* current step size */

        /* remove every ‘step’-th element and record its factor */
        size_t dst = 0;
        for (size_t idx = 0; idx < alive_size; ++idx) {
            if ((idx + 1) % step == 0) {
                if (alive[idx] <= N) fac[alive[idx]] = (seq_t)step;
            } else {
                alive[dst++] = alive[idx];
            }
        }
        alive_size = dst;

        /* decide next position:
           if the step element was removed (its original position
           divisible by step), keep pos the same; otherwise move to the
           next element. */
        if (((pos + 1) % step) != 0) {
            ++pos;
        }
    }

    free(alive);

    /* free old factor array and store the new one */
    if (g_factor) free(g_factor);
    g_factor = fac;
    g_max_n  = N;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset is 1; undefined below */
    compute_upto(n);                           /* lazy precomputation */
    return g_factor[n];
}

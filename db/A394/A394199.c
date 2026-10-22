/*
 * A394199  Expansion of Product_{k>=1} (1 + x^(k*(k+3)/2)) / (1 -
 *          x^(k*(k+3)/2)).
 * https://oeis.org/A394199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of the generating function *
 *            Product_{k>=1} (1 + x^(k*(k+3)/2)) / (1 - x^(k*(k+3)/2)). * The
 *            computation is performed by iteratively convolving with each *
 *            factor (1 + 2*x^e + 2*x^(2e) + ...) where e = k*(k+3)/2, using *
 *            the recurrence * * new_a[n] = old_a[n] + 2 * Σ_{m=1....
 * Range:     u8 n <= 42, u16 n <= 127, u32 n <= 475, u64 n <= 2159,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free, calloc */
#include <stdint.h>   /* uint64_t */

seq_t a(idx_t n)
{
    /* allocate two work arrays of size n+1, zero‑initialized */
    size_t size = (size_t)(n + 1);
    seq_t *cur = (seq_t *)calloc(size, sizeof(seq_t));
    seq_t *nxt = (seq_t *)calloc(size, sizeof(seq_t));
    if (!cur || !nxt) {
        /* memory allocation failed – return 0 as a safe default */
        free(cur);
        free(nxt);
        return (seq_t)0;
    }

    /* base case: one way to form 0, none for positive n */
    cur[0] = (seq_t)1;          /* cur[0] = 1, cur[>0] already 0 */

    /* iterate over all e = k*(k+3)/2 that are <= n      */
    idx_t e = (idx_t)2;          /* e for k = 1 */
    idx_t k = (idx_t)1;

    while (e <= n) {
        /* for each residue class modulo e compute the recurrence */
        for (idx_t r = (idx_t)0; r < e; ++r) {
            seq_t sum = (seq_t)0;
            for (idx_t t = r; t <= n; t += e) {
                /* new = old + 2 * sum_of_predecessors */
                nxt[t] = cur[t] + ((seq_t)2) * sum;
                sum += cur[t];
            }
        }

        /* swap cur and nxt for next e */
        seq_t *tmp = cur;
        cur = nxt;
        nxt = tmp;

        /* advance to next part size: e_{k+1} = e_k + k + 2 */
        e += k + (idx_t)2;
        ++k;
    }

    /* result is in cur[n] */
    seq_t result = cur[n];

    /* free temporary arrays */
    free(cur);
    free(nxt);

    return result;
}

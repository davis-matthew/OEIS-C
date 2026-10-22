/*
 * A339101  Number of compositions (ordered partitions) of n into distinct
 *          parts >= 3.
 * https://oeis.org/A339101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n into distinct parts >=3
 * Range:     u8 n <= 22, u16 n <= 43, u32 n <= 94, u64 n <= 222,
 *            u128 n <= 569, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* maximum number of parts (k) needed for this n */
    idx_t k = 0;
    while ((k * (k + 5) / 2) <= n)
        ++k;
    if (k > 0) --k;  /* last k that satisfies the inequality */

    /* allocate working arrays */
    seq_t *answer = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *coeff  = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *fact   = (seq_t *)calloc((size_t)(k + 1), sizeof(seq_t));

    /* Base: k = 0 */
    coeff[0] = 1;
    fact[0]  = 1;
    answer[0] = 1;   /* T_0 = 0, k! = 1, coeff[0] = 1 */

    /* Incrementally build coefficients and accumulate answers */
    for (idx_t kk = 1; kk <= k; ++kk) {
        /* Update coeff array: multiply by 1/(1 - x^kk) */
        for (idx_t s = kk; s <= n; ++s)
            coeff[s] += coeff[s - kk];

        /* Compute T_k and factorial */
        idx_t T_k = (kk * (kk + 5)) / 2;
        fact[kk] = fact[kk - 1] * (seq_t)kk;

        /* Add contributions for this kk to answer[n] */
        for (idx_t s = T_k; s <= n; ++s)
            answer[s] += fact[kk] * coeff[s - T_k];
    }

    seq_t result = answer[n];
    free(answer);
    free(coeff);
    free(fact);
    return result;
}

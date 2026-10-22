/*
 * A375833  E.g.f. satisfies A(x) = 1/(1 - x*(exp(x^2*A(x)) - 1)).
 * https://oeis.org/A375833
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/2)} (n-k)! * Stirling2(k,n-2*k)/(
 *            k! * (k+1)! )
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Computes the nth term of OEIS A375833. */
seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0)
        return (seq_t)1;

    /* ensure non‑negative size_t for indexing */
    size_t N = (size_t)n;
    size_t maxK = N / 2;

    /* factorials 0! … N! */
    seq_t *fact = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!fact)
        return (seq_t)0;
    fact[0] = (seq_t)1;
    for (size_t i = 1; i <= N; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* Stirling–2 numbers: only rows 0 … maxK are required */
    seq_t *prev = (seq_t *)malloc((maxK + 1) * sizeof(seq_t));
    seq_t *curr = (seq_t *)malloc((maxK + 1) * sizeof(seq_t));
    if (!prev || !curr) {
        free(fact);
        free(prev);
        free(curr);
        return (seq_t)0;
    }
    /* row n = 0 : S(0,0) = 1, all other entries 0 */
    for (size_t j = 0; j <= maxK; ++j)
        prev[j] = (seq_t)0;
    prev[0] = (seq_t)1;

    seq_t result = (seq_t)0;

    for (size_t k = 0; k <= maxK; ++k) {
        /* After this point, prev contains S(k,·) because for k>0 we computed it below. */
        size_t m = N - 2 * k;
        if (m <= k && prev[m] != (seq_t)0) {
            seq_t term = fact[N];                 /* n!          */
            term /= fact[k];                     /* divide by k! */
            term /= fact[k + 1];                 /* divide by (k+1)! */
            term *= fact[N - k];                 /* multiply by (n-k)! */
            term *= prev[m];                     /* multiply by S(k,m) */
            result += term;
        }

        /* Prepare next Stirling row only if we are not at the last k */
        if (k < maxK) {
            curr[0] = (seq_t)0;
            for (size_t j = 1; j <= k + 1; ++j) {
                seq_t addend = (j <= k) ? ((seq_t)j * prev[j]) : (seq_t)0;
                curr[j] = addend + prev[j - 1];
            }
            /* swap prev and curr for the next iteration */
            seq_t *tmp = prev;
            prev = curr;
            curr = tmp;
        }
    }

    free(fact);
    free(prev);
    free(curr);

    return result;
}

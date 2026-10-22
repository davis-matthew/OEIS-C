/*
 * A333797  Total number of saturated chains in the lattices L_n(2) of
 *          subspaces (ordered by inclusion) of the vector space GF(2)^n.
 * https://oeis.org/A333797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0}^{n} [n choose k]_2 * g_k, where g_n = 1 and g_k =
 *            1 + (2^{n-k} – 1) * g_{k+1}. The q‑binomial coefficient
 *            satisfies [n choose k]_2 = [n choose k-1]_2 * (2^{n-k+1} – 1) /
 *            (2^{k} – 1). This implementation computes the sum iteratively,
 *            using bit‑shift to evaluate 2^{d}·x – x efficient...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
 
seq_t a(idx_t n) {
    /* convert n to a signed int for loop indices (n <= 10000 is safe) */
    int N = (int)n;
    if (N == 0) {
        return (seq_t)1;
    }
 
    /* Allocate array of powers of two (2^i) */
    seq_t *pow2 = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!pow2) return (seq_t)0;   /* allocation failure handling omitted */
    pow2[0] = (seq_t)1;
    for (int i = 1; i <= N; ++i) {
        pow2[i] = pow2[i-1] * (seq_t)2;
    }
 
    seq_t sum = (seq_t)0;
    seq_t g_next = (seq_t)1;     /* g_N = 1 */
    seq_t binom_next = (seq_t)1; /* binom_N = 1 */
 
    /* k == N case */
    sum += g_next * binom_next;  /* contributes a(N) */
 
    /* Iterate k from N-1 down to 0 */
    for (int k = N - 1; k >= 0; --k) {
        /* compute g_k = 1 + (2^{N-k} - 1) * g_{k+1} using shift */
        size_t shift = (size_t)(N - k);
        seq_t shifted = g_next << shift;   /* g_{k+1} * 2^{shift} */
        seq_t temp = shifted - g_next;     /* g_{k+1} * (2^{shift} - 1) */
        seq_t g_k = temp + (seq_t)1;
 
        /* compute binom_k = binom_{k+1} * (2^{k+1} - 1) / (2^{N-k} - 1) */
        seq_t denom = pow2[shift] - (seq_t)1;
        seq_t numer = pow2[k + 1] - (seq_t)1;
        seq_t binom_k = binom_next * numer / denom;
 
        sum += binom_k * g_k;
 
        /* update for next iteration */
        g_next = g_k;
        binom_next = binom_k;
    }
 
    free(pow2);
    return sum;
}

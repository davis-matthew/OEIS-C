/*
 * A306511  Number of permutations p of [n] having at least one index i with
 *          |p(i)-i| = 1.
 * https://oeis.org/A306511
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! – b(n) * b(n) = Σ_{k=0}^{⌊n/2⌋·2} (‑1)^k · M_k ·
 *            (n‑k)! * M_k is the number of matchings of size k in the
 *            forbidden‑cell bipartite graph. * For each component
 *            (even‑left/odd‑right or odd‑left/even‑right) the number of *
 *            matchings of size i is C(n‑i,i). Hence M_k = Σ_{i=0}^k
 *            C(n‑i,i)·C(n...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* ---------- factorials ------------------------------------------------ */
    seq_t *fact = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        fact[i] = fact[i - 1] * ((seq_t)i);

    /* ---------- matchings for one component ----------------------------- */
    idx_t kmax = n / 2;                 /* maximum size of a matching in one component */
    seq_t *g    = (seq_t*)calloc((size_t)kmax + 1, sizeof(seq_t)); /* g[i] = C(n-i,i) */
    g[0] = (seq_t)1;
    for (idx_t i = 1; i <= kmax; ++i) {
        /* g[i] = g[i-1] * (n-2*i+2)*(n-2*i+1) / (i*(n-i+1)) */
        seq_t tmp = g[i - 1] * (seq_t)(n - 2 * i + 2);
        tmp /= (seq_t)i;
        tmp *= (seq_t)(n - 2 * i + 1);
        tmp /= (seq_t)(n - i + 1);
        g[i] = tmp;
    }

    /* ---------- convolution g*g -> total matchings --------------------- */
    idx_t kk   = 2 * kmax;                     /* maximum total matching size */
    seq_t *C   = (seq_t*)calloc((size_t)kk + 1, sizeof(seq_t));
    for (idx_t i = 0; i <= kmax; ++i) {
        for (idx_t j = 0; j <= kmax; ++j) {
            seq_t prod = g[i] * g[j];
            C[i + j] += prod;
        }
    }

    /* ---------- inclusion–exclusion for b(n) --------------------------- */
    seq_t plus  = (seq_t)0;
    seq_t minus = (seq_t)0;
    for (idx_t k = 0; k <= kk && k <= n; ++k) {
        seq_t term = C[k] * fact[n - k];
        if ((k & 1) == 0)
            plus += term;
        else
            minus += term;
    }
    seq_t b = plus - minus;   /* number of permutations with no |p(i)-i|=1 */

    seq_t result = fact[n] - b;   /* desired a(n) */

    /* ---------- clean up ------------------------------------------------- */
    free(fact);
    free(g);
    free(C);

    return result;
}

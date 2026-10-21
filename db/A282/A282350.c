/*
 * A282350  Expansion of (Sum_{k>=0} x^(k*(5*k^2-5*k+2)/2))^15.
 * https://oeis.org/A282350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered 15‑tuples of icosahedral numbers * that
 *            sum to n, i.e. coefficient of x^n in * (Sum_{k≥0}
 *            x^{k*(5*k^2-5*k+2)/2})^15 * Implementation: dynamic programming
 *            on 15 convolutions of the * icosahedral number set, cached
 *            globally so that * repeated calls with arbitrary n work eff...
 * Range:     u8 n <= 2, u16 n <= 27, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free, size_t */
#include <stdint.h>   /* uint64_t, if needed */

#if defined(__cplusplus)
#  define ALLOC_SEQ_T(n) new seq_t[(n)]
#  define FREE_SEQ_T(p)  delete[] (p)
#  define ALLOC_IDX_T(n) new idx_t[(n)]
#  define FREE_IDX_T(p)  delete[] (p)
#else
#  define ALLOC_SEQ_T(n) ((seq_t*)malloc((size_t)(n)*sizeof(seq_t)))
#  define FREE_SEQ_T(p)  free((p))
#  define ALLOC_IDX_T(n) ((idx_t*)malloc((size_t)(n)*sizeof(idx_t)))
#  define FREE_IDX_T(p)  free((p))
#endif

/**
 *  Return the coefficient a(n) for the OEIS sequence A282350.
 *  The result is cached: if a larger n is requested, the whole
 *  DP table is recomputed up to that n; otherwise the cached
 *  values are reused.
 */
seq_t a(idx_t n) {
    /* cache state */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;      /* number of entries in dp (max_n+1) */

    /* recompute DP table if needed */
    if (dp == NULL || n >= dp_size) {
        /* free old cache if present */
        FREE_SEQ_T(dp);

        /* ----------  compute icosahedral numbers up to n ---------- */
        /* first count how many terms are needed */
        idx_t k = 0;
        idx_t count = 0;
        while (1) {
            seq_t kk = (seq_t)k;
            seq_t inner = (seq_t)5 * kk * kk - (seq_t)5 * kk + (seq_t)2;
            seq_t f = kk * inner / 2;
            if (f > (seq_t)n) break;
            count++;
            k++;
        }

        /* allocate array for icosahedral values */
        idx_t *icosa = ALLOC_IDX_T(count);
        k = 0;
        idx_t idx = 0;
        while (idx < count) {
            seq_t kk = (seq_t)k;
            seq_t inner = (seq_t)5 * kk * kk - (seq_t)5 * kk + (seq_t)2;
            seq_t f = kk * inner / 2;
            if (f > (seq_t)n) break;
            icosa[idx++] = (idx_t)f;
            k++;
        }

        /* ----------  dynamic programming for 15 convolutions  ---------- */
        size_t N = (size_t)n + 1;          /* array size */
        dp = ALLOC_SEQ_T(N);
        for (size_t i = 0; i < N; ++i) dp[i] = 0;
        dp[0] = 1;                         /* 0 numbers sum to 0 in 1 way */

        for (idx_t t = 0; t < 15; ++t) {
            seq_t *newdp = ALLOC_SEQ_T(N);
            for (size_t i = 0; i < N; ++i) newdp[i] = 0;

            for (idx_t fi = 0; fi < count; ++fi) {
                idx_t f = icosa[fi];
                if (f > n) continue;
                size_t sf = (size_t)f;
                for (size_t s = sf; s < N; ++s) {
                    newdp[s] += dp[s - sf];
                }
            }

            FREE_SEQ_T(dp);
            dp = newdp;
        }

        FREE_IDX_T(icosa);
        dp_size = (idx_t)n + 1;
    }

    return dp[n];
}

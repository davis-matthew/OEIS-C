/*
 * A396505  A sequence constructed by greedily sampling the probability
 *          distribution given by log_2(1+1/i)^2 - log_2(1+1/(i+1))^2 to
 *          minimize discrepancy.
 * https://oeis.org/A396505
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sequence A396505 – greedy sampling of probability distribution
 *            p(k) = log2(1+1/k)^2 – log2(1+1/(k+1))^2 to minimise
 *            discrepancy. The function a(idx_t n) returns the n‑th term
 *            (1‑indexed)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define MAX_K 5000   /* Upper bound on k needed for n ≤ 10000 */

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static idx_t max_computed = 0;      /* number of terms already computed */
    static seq_t *seq = NULL;           /* sequence array (1‑indexed) */
    static size_t seq_cap = 0;          /* capacity of seq */
    static int *cnt = NULL;             /* counts of each k (1‑indexed) */
    static double *pr = NULL;           /* pre‑computed probabilities p(k) */

    /* ---------- initialise helpers on first call ---------- */
    if (!pr) {
        pr = (double*)malloc((MAX_K + 1) * sizeof(double));
        for (int k = 1; k <= MAX_K; ++k) {
            double a1 = log(1.0 + 1.0 / k) / log(2.0);
            double a2 = log(1.0 + 1.0 / (k + 1)) / log(2.0);
            pr[k] = a1 * a1 - a2 * a2;
        }
        cnt = (int*)calloc(MAX_K + 1, sizeof(int));
    }

    /* ---------- grow sequence storage if needed ---------- */
    if (seq_cap < n) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 256;
        while (new_cap < n) new_cap *= 2;
        seq = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }

    /* ---------- generate missing terms ---------- */
    for (idx_t t = max_computed + 1; t <= n; ++t) {
        int best_k = 1;
        double best_val = (double)t * pr[1] - cnt[1];

        for (int k = 2; k <= MAX_K; ++k) {
            double val = (double)t * pr[k] - cnt[k];
            if (val > best_val) {
                best_val = val;
                best_k   = k;
            }
        }

        seq[t - 1] = (seq_t)best_k;
        ++cnt[best_k];
    }

    max_computed = n;
    return seq[n - 1];
}

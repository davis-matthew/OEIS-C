/*
 * A400399  Value of k that maximizes P(X < Y), where X and Y are independent
 *          binomial random variables with k trials and success probabilities
 *          1/n and 1/(2*n), respectively. In case of ties, choose the largest
 *          k.
 * https://oeis.org/A400399
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, new
 * Formula:   a(n) = k that maximizes P(X < Y), where X ~ Bin(k,1/n), Y ~
 *            Bin(k,1/(2n)). The value is the integer k (≥2) giving the
 *            largest probability, ties broken by the largest k. The
 *            implementation evaluates the probability for a small range
 *            around the linear approximation k ≈ 1.46 n and chooses the
 *            optimu...
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef NEG_INF
#define NEG_INF (-HUGE_VALL)
#endif

/* Compute log10 of probability that X < Y for given k and n.
   Long‑double arithmetic with log‑sum‑exp is used to keep
   relative comparisons accurate. */
static long double logP_k(size_t k, idx_t n)
{
    long double ln_pX  = logl(1.0L / (long double)n);
    long double ln_qX  = logl(1.0L - 1.0L / (long double)n);
    long double ln_pY  = logl(1.0L / (2.0L * (long double)n));
    long double ln_qY  = logl(1.0L - 1.0L / (2.0L * (long double)n));

    /* Allocate arrays.  For n up to about 10000, k < 20000, arrays remain tiny. */
    long double *lnP_X = (long double*)malloc((k + 1) * sizeof(long double));
    long double *lnP_Y = (long double*)malloc((k + 1) * sizeof(long double));
    long double *tail  = (long double*)malloc((k + 2) * sizeof(long double));
    if (!lnP_X || !lnP_Y || !tail) return NEG_INF;   /* out of memory → ignore */

    /* X distribution */
    lnP_X[0] = (long double)k * ln_qX;
    for (size_t i = 0; i < k; ++i) {
        long double term = logl((long double)(k - i)) - logl((long double)(i + 1))
                          + (ln_pX - ln_qX);
        lnP_X[i + 1] = lnP_X[i] + term;
    }

    /* Y distribution */
    lnP_Y[0] = (long double)k * ln_qY;
    for (size_t i = 0; i < k; ++i) {
        long double term = logl((long double)(k - i)) - logl((long double)(i + 1))
                          + (ln_pY - ln_qY);
        lnP_Y[i + 1] = lnP_Y[i] + term;
    }

    /* tail[i] = log( Σ_{j=i}^{k} P(Y=j) ) */
    tail[k] = lnP_Y[k];
    for (size_t i = k; i --> 0; ) {
        long double a = tail[i + 1];
        long double b = lnP_Y[i];
        if (a > b) tail[i] = a + log1pl(expl(b - a));
        else       tail[i] = b + log1pl(expl(a - b));
    }
    tail[k + 1] = NEG_INF;    /* for i==k the tail sum over j>k is zero */

    /* Sum over i: P(X=i) * P(Y>i) */
    long double log_sum = NEG_INF;
    for (size_t i = 0; i <= k; ++i) {
        long double term_log = lnP_X[i] + (i < k ? tail[i + 1] : NEG_INF);
        if (log_sum == NEG_INF) {
            log_sum = term_log;
        } else if (term_log > log_sum) {
            log_sum = term_log + log1pl(expl(log_sum - term_log));
        } else {
            log_sum = log_sum + log1pl(expl(term_log - log_sum));
        }
    }

    free(lnP_X);
    free(lnP_Y);
    free(tail);
    return log_sum;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* undefined below offset 2 */

    /* Approximate linear range around the limiting constant */
    size_t k0 = (size_t)((long double)n * 1.46L - 0.292L);
    if (k0 < 1) k0 = 1;

    size_t best_k = k0;
    long double best_log = NEG_INF;
    const long double eps = 1e-18L;    /* tie threshold in log space */

    for (long long d = -3; d <= 3; ++d) {
        long long cand = (long long)k0 + d;
        if (cand < 1) continue;
        if (cand > 5 * n) cand = 5 * n;      /* safety upper bound */
        size_t k = (size_t)cand;

        long double cur_log = logP_k(k, n);
        if (cur_log > best_log || (fabsl(cur_log - best_log) <= eps && k > best_k)) {
            best_log = cur_log;
            best_k = k;
        }
    }
    return (seq_t)best_k;
}

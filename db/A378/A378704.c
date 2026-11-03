/*
 * A378704  Array read by ascending antidiagonals: A(n, k) is the total area
 *          of n-Fibonacci polyominoes with k columns, where k > 0.
 * https://oeis.org/A378704
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the array read by ascending antidiagonals of the
 *            triangle A(n,k) defined by the generating function A(n, k) =
 *            [y^k] y*(n^2*(1 - y)^2*y^n + 2*y*(1 - y^n) - n(1 - y)*(2- y^n +
 *            y^(n+1)))/(2*(-1 + y)*(1 - 2*y + y^(n+1))^2). The function below
 *            computes a(n) directly from this definition
 * Range:     u8 n <= 20, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 67 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: compute A(nrow, kcol) for n>=2, k>=1 */
static seq_t compute_A_nk(idx_t nrow, idx_t kcol)
{
    /* Maximum k needed in a single call is small (≤~140 for n≤10000),
       so a fixed-size buffer of 200 entries is sufficient. */
    enum { MAX_K = 200 };
    seq_t s[MAX_K + 1];
    seq_t t[MAX_K + 1];
    seq_t pos[MAX_K + 1];
    seq_t neg[MAX_K + 1];
    int k = (int)kcol;           /* local int is fine for small k */
    int i, j;

    /* --- Compute coefficients of 1 / Q(y) where Q(y) = 1 - 2y + y^(n+1) --- */
    s[0] = 1;
    seq_t nseq = (seq_t)nrow;
    for (i = 1; i <= k; ++i) {
        seq_t val = 2 * s[i - 1];
        if (i >= (int)(nrow + 1))
            val -= s[i - (int)(nrow + 1)];
        s[i] = val;
    }

    /* --- Compute t(y) = (1 / Q(y))^2  as a power series --- */
    t[0] = 1;
    for (i = 1; i <= k; ++i) {
        t[i] = 0;
        for (j = 0; j <= i; ++j)
            t[i] += s[j] * s[i - j];
    }

    /* --- Initialize positive/negative accumulators --- */
    for (i = 0; i <= k; ++i) {
        pos[i] = 0;
        neg[i] = 0;
    }

    /* --- Positive coefficient contributions --- */
    /* degree 2: coeff = 2*(1+n) */
    if (k >= 2) {
        seq_t coeff = 2 * (1 + nseq);
        for (i = 2; i <= k; ++i)
            pos[i] += coeff * t[i - 2];
    }
    /* degree n+1: coeff = n*(n+1) */
    if (nrow + 1 <= k) {
        seq_t coeff = nseq * (nseq + 1);
        for (i = (int)(nrow + 1); i <= k; ++i)
            pos[i] += coeff * t[i - (int)(nrow + 1)];
    }
    /* degree n+3: coeff = n*(n+1) */
    if (nrow + 3 <= k) {
        seq_t coeff = nseq * (nseq + 1);
        for (i = (int)(nrow + 3); i <= k; ++i)
            pos[i] += coeff * t[i - (int)(nrow + 3)];
    }

    /* --- Negative coefficient contributions (stored as positive) --- */
    /* degree 1: coeff = 2*n */
    if (k >= 1) {
        seq_t coeff = 2 * nseq;
        for (i = 1; i <= k; ++i)
            neg[i] += coeff * t[i - 1];
    }
    /* degree n+2: coeff = 2*(n^2 + n + 1) */
    if (nrow + 2 <= k) {
        seq_t coeff = 2 * ((nseq * nseq) + nseq + 1);
        for (i = (int)(nrow + 2); i <= k; ++i)
            neg[i] += coeff * t[i - (int)(nrow + 2)];
    }

    /* --- Prefix sums of positive and negative contributions --- */
    for (i = 1; i <= k; ++i) {
        pos[i] += pos[i - 1];
        neg[i] += neg[i - 1];
    }

    /* The required coefficient sum is diff = neg[k] - pos[k];
       it is guaranteed to be even, as the final result is an integer. */
    seq_t diff = neg[k] - pos[k];
    return diff / 2;
}

/* Public function: a(n) for the OEIS sequence A378704. */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;              /* behaviour below offset is unspecified */

    idx_t idx = n - 2;          /* zero‑based index in the anti‑diagonal order */

    /* Find the antidiagonal d such that the cumulative number of terms
       up to d-1 is < idx and up to d is ≥ idx+1.  Here d = current sum n+k. */
    idx_t d = 3;
    while ((d - 2) * (d - 1) / 2 <= idx)
        ++d;

    idx_t prev_cum = (d - 3) * (d - 2) / 2;   /* terms before diagonal d */
    idx_t offset = idx - prev_cum;            /* position within diagonal d */

    idx_t row = (d - 1) - offset;   /* value of n in A(n,k) */
    idx_t col = offset + 1;         /* value of k in A(n,k) */

    return compute_A_nk(row, col);
}

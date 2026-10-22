/*
 * A350481  G.f. A(x) satisfies: A(x) = A(x^3 - x^6)/x^2.
 * https://oeis.org/A350481
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1}^{⌊(n+2)/3⌋} a(k) * (-1)^{n-3k+2} * C(3k-2,
 *            n-3k+2) Sequence A350481. Computation is based on the functional
 *            equation A(x) = A(x^3 - x^6)/x^2. Binomial coefficients are
 *            generated incrementally to avoid repeated recomputation. The
 *            routine works for any seq_t and idx_t provided by t...
 * Range:     i8 n <= 12, i16 n <= 21, i32 n <= 37, i64 n <= 37, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < (idx_t)1) return (seq_t)0;          /* defined only for n ≥ 1 */

    size_t N = (size_t)n;                       /* convert to portable size_t */

    /* allocate array for the sequence terms (index 0 unused) */
    seq_t *seq = (seq_t *)malloc(sizeof(seq_t) * (N + 1));
    if (!seq) return (seq_t)0;                  /* allocation failed */

    for (size_t i = 0; i <= N; ++i) seq[i] = (seq_t)0;
    if (N >= 1) seq[1] = (seq_t)1;              /* base value a(1) = 1 */

    /* precompute maximum k needed */
    size_t maxK = (N + 2) / 3;                  /* floor((N+2)/3) */
    size_t maxBinomSize = 3 * maxK;             /* enough for all rows */

    seq_t *binom = (seq_t *)malloc(sizeof(seq_t) * maxBinomSize);
    if (!binom) { free(seq); return (seq_t)0; }

    /* main loop over k */
    for (size_t k = 1; k <= maxK; ++k) {
        size_t k2 = 3 * k - 2;                   /* 3k-2 */

        /* build binomial coefficients for the row n = 3k-2 */
        binom[0] = (seq_t)1;
        for (size_t r = 1; r <= k2; ++r) {
            seq_t prev   = binom[r - 1];
            seq_t numer  = (seq_t)(k2 - r + 1) * prev;
            binom[r] = numer / (idx_t)r;        /* exact division */
        }

        seq_t a_k = seq[k];                      /* current a(k) already final */

        /* add contributions to all n that depend on this k */
        for (size_t s = 0; s <= k2; ++s) {
            size_t nIdx = 3 * k - 2 + s;
            if (nIdx > N) break;
            if (nIdx == k) continue;             /* skip the self‑term for k=1 */

            seq_t term = a_k * binom[s];
            if (s & 1) term = -term;             /* (-1)^s factor */
            seq[nIdx] += term;
        }
    }

    seq_t result = seq[N];

    free(binom);
    free(seq);
    return result;
}

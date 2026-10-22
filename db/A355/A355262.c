/*
 * A355262  Array of Fuss-Catalan numbers read by ascending antidiagonals,
 *          A(n, k) = binomial(k*n + 1, k)/(k*n + 1).
 * https://oeis.org/A355262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th element read by ascending antidiagonals from
 *            the Fuss‑Catalan array A(n,k) = binomial(k*n + 1, k)/(k*n + 1)
 * Range:     u8 n <= 23, u16 n <= 49, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* See OEIS A355262. */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* Find the anti‑diagonal S so that more than n entries have been
       accumulated after diagonals 0..S-1.
       Cumulative count of entries up to diagonal d is (d+1)(d+2)/2. */
    idx_t S = 0;
    for (;;) {
        idx_t cum = (S + 1) * (S + 2) / 2;
        if (cum > n)
            break;
        ++S;
    }

    /* Number of entries before diagonal S */
    idx_t prev = S * (S + 1) / 2;

    /* Position within diagonal S:
       k ranges from 0 to S, u = S - k.        */
    idx_t k = n - prev;
    idx_t u = S - k;

    /* Compute A(u,k) = binomial(k*u + 1, k)/(k*u + 1). */
    idx_t denom = k * u + 1;       /* denom >= 1 */
    if (k > denom)                /* binomial(denom, k) = 0 when k > denom */
        return (seq_t)0;

    seq_t num = binomial(denom, (idx_t)k); /* binomial returns seq_t */
    return num / (seq_t)denom;              /* exact division */
}

/*
 * A333446  Table T(n,k) read by upward antidiagonals. T(n,k) = Sum_{i=1..n}
 *          Product_{j=1..k} (i-1)*k+j.
 * https://oeis.org/A333446
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(n',k') T(n,k) = Σ_{i=1..n} ∏_{j=1..k} ((i-1)*k + j) The
 *            index n is mapped to a pair (n',k') as follows: • Let d be the
 *            minimal integer with (d-1)*d/2 ≥ n. • start index of diagonal d
 *            is (d-2)*(d-1)/2 + 1. • offset = n - start (0‑based within the
 *            diagonal). • n' = d-1 - offset, k = d - n'
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 51, u64 n <= 116,
 *            u128 n <= 295, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t N)
{
    /* Find diagonal d such that indices up to this diagonal end at (d-1)*d/2. */
    idx_t d = 2;
    while (1) {
        unsigned __int128 tri = (unsigned __int128)(d - 1) * d / 2;
        if (tri >= N) break;
        ++d;
    }

    /* First index on diagonal d */
    idx_t start = ( (d - 2) * (d - 1) ) / 2 + 1;

    /* Offset of N within diagonal (0‑based) */
    idx_t offset = N - start;                /* 0 ≤ offset ≤ d‑2 */

    /* Coordinates (n',k') on the table */
    idx_t nprime = d - 1 - offset;            /* 1 ≤ nprime ≤ d‑1 */
    idx_t k      = d - nprime;                /* 1 ≤ k ≤ d‑1 */

    /* Compute T(n',k') */
    seq_t sum = 0;
    for (idx_t i = 1; i <= nprime; ++i) {
        seq_t prod = 1;
        idx_t base = (i - 1) * k;            /* starting value of the k‑term window */
        for (idx_t j = 1; j <= k; ++j) {
            prod *= (seq_t)(base + j);
        }
        sum += prod;
    }
    return sum;
}

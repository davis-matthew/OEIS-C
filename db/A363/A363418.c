/*
 * A363418  Square array read by ascending antidiagonals: T(n,k) = [x^(n*k)]
 *          ((1 + x)/(1 - x))^k for n, k >= 1.
 * https://oeis.org/A363418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(i,k) read by ascending antidiagonals of the square
 *            array * T(i,k) = Sum_{j=0..k} binom(k,j) * binom((i+1)*k - j -
 *            1, i*k - j) * The index n is converted to row i and column k as
 *            follows: * 1. Find diagonal d with d(d+1)/2 ≥ n. * 2. position =
 *            n - (d−1)d/2 − 1 (0‑based in that diagonal)....
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 73, u64 n <= 184,
 *            u128 n <= 520, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

static seq_t compute_T(idx_t i, idx_t k)
{
    seq_t sum = 0;
    for (idx_t j = 0; j <= k; ++j) {
        seq_t term = binom(k, j);
        idx_t a = (i + 1) * k - j - 1;
        idx_t b = i * k - j;
        term = term * binom(a, b);
        sum += term;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Find diagonal d such that d(d+1)/2 >= n */
    idx_t d = 0;
    unsigned __int128 acc = 0;
    while (acc < n) {
        ++d;
        acc += d;
    }

    /* Number of terms before this diagonal */
    idx_t prev = (idx_t)(acc - d);
    /* 0‑based position inside the diagonal */
    idx_t offset = n - prev - 1;
    /* Coordinates in the array */
    idx_t i = d - offset;   /* row */
    idx_t k = offset + 1;   /* column */

    return compute_T(i, k);
}

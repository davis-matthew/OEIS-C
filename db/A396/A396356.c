/*
 * A396356  Triangle of convolved numbers of the 3-section of the Fibonacci
 *          sequence.
 * https://oeis.org/A396356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(m,k) = Sum_{r=0..floor((m-1)/2)} * binomial(k+m-1-r, k) *
 *            binomial(m-1-r, r) * 4^(m-1-2r) * a(n) = A(n-s+1, s) where s is
 *            the column index in the triangle. * (The triangle entries are
 *            taken row‑by‑row, starting with s = 1 in each row.)
 * Range:     u8 n <= 6, u16 n <= 28, u32 n <= 105, u64 n <= 382,
 *            u128 n <= 1545, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Find the row i (number of terms in previous rows is (i-1)*i/2). */
    idx_t i = 1;
    while ((seq_t)i * (i + 1) / 2 < (seq_t)n)
        ++i;

    /* Column index j (1‑based) in this row. */
    idx_t prev = (i - 1) * i / 2;
    idx_t j = n - prev;          /* j ∈ [1..i] */

    /* Convert to A(m,k) parameters. */
    idx_t m = i - j + 1;         /* m ≥ 1 */
    idx_t k = j;                 /* k ≥ 1 */

    /* Compute A(m,k) by the sum formula. */
    seq_t sum = 0;
    idx_t max_r = (m - 1) / 2;   /* floor((m-1)/2) */
    for (idx_t r = 0; r <= max_r; ++r) {
        seq_t b1 = binomial(k + m - 1 - r, k);
        seq_t b2 = binomial(m - 1 - r, r);

        /* Compute 4^(m-1-2r) by repeated multiplication. */
        seq_t pow4 = 1;
        idx_t exp = m - 1 - 2 * r;
        for (idx_t p = 0; p < exp; ++p)
            pow4 *= 4;

        seq_t term = b1;
        term *= b2;
        term *= pow4;
        sum += term;
    }
    return sum;
}

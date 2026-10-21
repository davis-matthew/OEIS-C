/*
 * A298122  a(n) is the number of n nonintersecting arches above the x-axis
 *          that start and/or end with an arch length equal to one and have
 *          floor((n+2)/2) arches starting in odd numbered positions.
 * https://oeis.org/A298122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   for n ≥ 1 a(1) = a(2) = 1, a(3) = 2 a(n) =
 *            2*(C(n-1,⌊(n-1)/2⌋)*C(n-2,⌊(n-2)/2⌋) -
 *            C(n-1,⌊(n-2)/2⌋)*C(n-2,⌊(n-3)/2⌋)) -
 *            (C(n-3,⌊(n-1)/2⌋)*C(n-2,⌊(n-1)/2⌋))/(⌊(n-1)/2⌋+1)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 21, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Compute binomial coefficient C(n,k) for 0 ≤ k ≤ n.
   Uses a multiplicative formula; intermediate results stay
   within the exactness limits of seq_t. */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k == 0 || k == n) return (seq_t)1;
    if (k > n - k) k = n - k;          /* Use symmetry to reduce loops */
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        /* Multiply before dividing to keep integer arithmetic exact. */
        res = (res * ((seq_t)(n - k + i))) / ((seq_t)i);
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Handle the small base cases directly. */
    if (n == 1 || n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)2;

    /* Floor values used in the formula */
    idx_t f1 = (n - 1) / 2;   /* ⌊(n-1)/2⌋ */
    idx_t f2 = (n - 2) / 2;   /* ⌊(n-2)/2⌋ */
    idx_t f3 = (n - 3) / 2;   /* ⌊(n-3)/2⌋ */

    /* Binomial coefficients */
    seq_t c1 = binom(n - 1, f1);   /* C(n-1, f1) */
    seq_t c2 = binom(n - 2, f2);   /* C(n-2, f2) */
    seq_t c3 = binom(n - 1, f2);   /* C(n-1, f2) */
    seq_t c4 = binom(n - 2, f3);   /* C(n-2, f3) */
    seq_t c5 = binom(n - 3, f1);   /* C(n-3, f1) */
    seq_t c6 = binom(n - 2, f1);   /* C(n-2, f1) */

    /* First part: 2*(c1*c2 - c3*c4) */
    seq_t prod1 = c1 * c2;
    seq_t prod2 = c3 * c4;
    seq_t part1 = (seq_t)2 * (prod1 - prod2);

    /* Second part: (c5*c6)/(f1+1) */
    seq_t denom = (seq_t)(f1 + 1);
    seq_t part2 = (c5 * c6) / denom;

    /* Result */
    return part1 - part2;
}

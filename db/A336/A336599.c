/*
 * A336599  Triangle read by rows: T(n,k) is the number of linear chord
 *          diagrams on 2n vertices with one marked chord such that exactly k
 *          of the remaining n-1 chords are contained within the marked chord.
 * https://oeis.org/A336599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 55, u64 n <= 136, u128 n <= 406,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A336599, triangle T(n,k) of linear chord diagrams.
   a(n) is the n-th element of the triangle read by rows.
   T(n,k) = Σ_{s=2k}^{min(n-1+k, 2n-2)} (2n-1-s)
             * C(s, 2k) * (2k-1)!! * C(2n-s-2, s-2k)
             * (s-2k)! * (2(n-1-s+k)-1)!!   */

#include "../utils/binomial.h"
#include "../utils/factorial.h"

static inline seq_t odd_double_factorial(idx_t m)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= m; ++i)
        res *= (seq_t)(2 * i - 1);
    return res;
}

static inline seq_t compute_T(idx_t n, idx_t k)
{
    seq_t result = 0;
    idx_t max_s = n - 1 + k;
    if (max_s > 2 * n - 2)
        max_s = 2 * n - 2;

    seq_t odd_df_k = odd_double_factorial(k);

    for (idx_t s = 2 * k; s <= max_s; ++s) {
        idx_t r   = s - 2 * k;                 /* number of crossing chords   */
        idx_t S   = 2 * n - s - 2;             /* number of exterior points   */
        idx_t t   = n - 1 - s + k;             /* number of exterior chords   */

        seq_t term = (seq_t)(2 * n - 1 - s);

        term *= binomial(s, 2 * k);
        term *= odd_df_k;
        term *= binomial(S, r);
        term *= factorial(r);
        term *= odd_double_factorial(t);

        result += term;
    }
    return result;
}

seq_t a(idx_t n)
{
    idx_t row = 1;
    while ((row * (row + 1)) / 2 < n)
        ++row;

    idx_t prev_tri = (row - 1) * row / 2;
    idx_t col      = n - prev_tri - 1; /* 0‑based column */

    return compute_T(row, col);
}

/*
 * A348593  Triangle read by rows: T(n,m) = Sum_{j=0..min(m,n-m)}
 *          C(2j,j)*C(n-2j-1,m-j)*C(n-m,j)/(j+1).
 * https://oeis.org/A348593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the entry of the triangle T(n,m) read by rows. * T(n,m)
 *            = Σ_{j=0..min(m,n‑m)} C(2j,j)·C(n‑2j‑1, m‑j)·C(n-m,j)/(j+1) *
 *            with rows indexed from n = 0. * The sequence a(n) is the
 *            flattening of the triangle: * n = 0 → T(0,0) * n = 1 → T(1,0) *
 *            n = 2 → T(1,1) * n = 3 → T(2,0) * n = 4 → T(2,1) *
 * Range:     u8 n <= 32, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   binomial, start_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/start_index.h"

/* C(n,k) for n ≥ 0, k ≥ 0, returning 0 when k > n.
 * Uses the supplied utilities/binomial.h which works on idx_t.
 */
static seq_t small_binom(idx_t n, idx_t k)
{
    if (k == 0 || n == k)
        return (seq_t)1;
    if (n < k)
        return (seq_t)0;
    return binomial(n, k);
}

/* C(n,k) with possible negative n.
 * For n < 0, the binomial is defined by:
 *     C(n,0) = 1, C(n,k) = 0 for k > 0.
 */
static seq_t small_binom_signed(int64_t n, idx_t k)
{
    if (n < 0)
        return (k == 0) ? (seq_t)1 : (seq_t)0;
    return small_binom((idx_t)n, k);
}

seq_t a(idx_t n)
{
    /* Find the row r such that start_index(r) ≤ n < start_index(r+1) */
    idx_t r = 0;
    seq_t next = start_index(1);
    while ((seq_t)n >= next) {
        ++r;
        next = start_index(r + 1);
    }

    /* Index within the row */
    seq_t row_start = start_index(r);
    idx_t c = (idx_t)((seq_t)n - row_start);  /* column (0‑based) */

    /* Sum the definition of T(r,c) */
    seq_t sum = (seq_t)0;
    idx_t jmax = (c < (r - c)) ? c : (r - c);
    for (idx_t j = 0; j <= jmax; ++j) {
        seq_t term = small_binom(2 * j, j);     /* C(2j,j) */
        term *= small_binom_signed((int64_t)r - 2 * (int64_t)j - 1,
                                  c - j);        /* C(r‑2j‑1, c‑j) */
        term *= small_binom_signed((int64_t)r - (int64_t)c, j); /* C(r‑c, j) */
        term /= ((seq_t)j + (seq_t)1);         /* divide by j+1 (exact) */
        sum += term;
    }
    return sum;
}

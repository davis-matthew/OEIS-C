/*
 * A325011  Triangle read by rows: T(n,k) is the number of achiral colorings
 *          of the facets of a regular n-dimensional orthotope using exactly k
 *          colors. Row n has 2n columns.
 * https://oeis.org/A325011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) is the triangle read by rows of T(n,k) where T(n,k) =
 *            Sum_{i=1}^{k} (-1)^{k-i} * binom(k,i) * binom(n + binom(i+1,2)-1
 *            , n) - Sum_{m=2}^{k} (-1)^{k-m} * binom(k,m) *
 *            binom(binomial(m,2), n) This implements the definition of
 *            A325011
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 52, u64 n <= 132, u128 n <= 132,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binom.h"

static seq_t compute_T(idx_t n, idx_t k)
{
    seq_t sum1_pos = 0, sum1_neg = 0;   /* parts of the first sum */
    seq_t sum2_pos = 0, sum2_neg = 0;   /* parts of the second sum */

    /* first summation : i = 1 .. k */
    for (idx_t i = 1; i <= k; ++i) {
        idx_t mirr = k - i;                      /* parity for the sign */
        seq_t term = binom(k, i) *
                     binom(n + (i + 1) * i / 2 - 1, n);
        if (mirr & 1U)
            sum1_neg += term;
        else
            sum1_pos += term;
    }

    /* second summation : m = 2 .. k (m = k-j) */
    for (idx_t m = 2; m <= k; ++m) {
        idx_t mirr = k - m;                      /* parity for the sign */
        seq_t term = binom(k, m) *
                     binom(m * (m - 1) / 2, n);
        if (mirr & 1U)
            sum2_neg += term;
        else
            sum2_pos += term;
    }

    /* T = (sum1_pos + sum2_neg) - (sum1_neg + sum2_pos) */
    seq_t total   = sum1_pos + sum2_neg;
    seq_t subtract= sum1_neg + sum2_pos;
    return total >= subtract ? total - subtract : 0;
}

/* Find the row r and column k for the flattened index idx (starting at 1). */
static void row_col(idx_t idx, idx_t *r, idx_t *k)
{
    /* Solve r(r+1) >= idx */
    long double d = sqrt((long double)(4.0L * idx + 1.0L));
    *r = (idx_t)((d - 1.0L) / 2.0L);

    /* Correct possible off‑by‑one due to rounding */
    while (*r * (*r + 1) < idx)  ++(*r);
    while (*r && (*r - 1) * (*r) >= idx)  --(*r);

    idx_t prev = (*r - 1) * (*r);   /* terms before this row */
    *k = idx - prev;                /* 1 .. 2r */
}

seq_t a(idx_t idx)
{
    idx_t r, k;
    row_col(idx, &r, &k);
    return compute_T(r, k);
}

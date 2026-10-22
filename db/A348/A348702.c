/*
 * A348702  Square array T(n, k) (n>=1, k>=1) read by antidiagonals upwards.
 *          T(n, k) is the number of partitions of the set [n] into lists of k
 *          noncrossing sets.
 * https://oeis.org/A348702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the square array * T(i,j) of size
 *            i>=1, j>=1 by antidiagonals upwards. * For a given (i,j) the
 *            array entry is * * T(i,j) = Σ_{r=1}^{min(i,j)} N(i,r) * (j! /
 *            (j-r)!) * * where N(i,r) are the Narayana numbers * N(i,r) =
 *            (1/i) * binom(i,r) * binom(i,r-1) * * with binom(m,...
 * Range:     u8 n <= 30, u16 n <= 70, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* --------------------------------------------------------------------
 *  Local helpers
 * -------------------------------------------------------------------- */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n)
        return (seq_t)0;
    if (k > n - k)
        k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        /* multiply by (n-k+i), then divide by i – exact at each step */
        res *= (seq_t)(n - k + i);
        res /= (seq_t)i;
    }
    return res;
}

/* --------------------------------------------------------------------
 *  Main sequence function
 * -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Find the antidiagonal index: sum s with (s-1)*s/2 >= n */
    idx_t s = 2;
    while ((s - 1) * s / 2 < n)
        ++s;

    /* Number of terms before diagonal s */
    idx_t prev = (s - 2) * (s - 1) / 2;

    /* Offset within the diagonal (0‑based) */
    idx_t offset = n - prev - 1;

    /* Antidiagonal coordinates (row i, column j) */
    idx_t i = s - 1 - offset;   /* 1‑based */
    idx_t j = offset + 1;       /* 1‑based */

    /* Sum over the number r of non‑empty blocks */
    idx_t max_r = (i < j) ? i : j;

    seq_t result = 0;

    for (idx_t r = 1; r <= max_r; ++r)
    {
        /* Narayana number N(i,r) = binom(i,r) * binom(i,r-1) / i */
        seq_t nar = binom(i, r);
        nar *= binom(i, r - 1);
        nar /= (seq_t)i;

        /* Factor j!/(j-r)! = product_{t=j-r+1}^{j} t */
        seq_t mult = 1;
        for (idx_t t = j - r + 1; t <= j; ++t)
            mult *= (seq_t)t;

        result += nar * mult;
    }

    return result;
}

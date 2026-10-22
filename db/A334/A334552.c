/*
 * A334552  Array read by antidiagonals: T(m,n) is the number of fixed
 *          polyominoes that have a width of m and height of n and m + n - 1
 *          cells.
 * https://oeis.org/A334552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 143, u32 n <= 542, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   binomial, find_sum_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A334552: Array read by antidiagonals.
 *
 *  The array entry T(m,n) counts fixed polyominoes of width m,
 *  height n and exactly m+n‑1 cells.
 *
 *  OEIS formula for T(m,n) (m>1, n>1):
 *      T(m,n) = 2*binom(m+n-2, m-1)
 *               + 2*(m+n-4)
 *               + (m-2)*(n-2)*(m+n-5)
 *               + 2*∑_{i=1}^{m-2}∑_{j=1}^{n-2}
 *                   ((m-2-i)*(n-2-j)+2) * binom(i+j,i)
 *
 *  For m=1 or n=1, T(m,n)=1.
 *
 *  The sequence a(n) reads the array along antidiagonals:
 *      a(1)=T(1,1), a(2)=T(1,2), a(3)=T(2,1), a(4)=T(1,3), ...
 *
 *  This implementation computes T(m,n) by the above formula
 *  (avoiding subtraction to keep all intermediate values non‑negative)
 *  and then maps the index n to (m,n) on the requested antidiagonal.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/find_sum_index.h"

/* Helper: compute T(m,n) by the formula above (no negative intermediates). */
static seq_t compute_T(idx_t m, idx_t n)
{
    if (m <= 1 || n <= 1) {
        return (seq_t)1;
    }

    /* 2 * binomial(m+n-2, m-1) */
    seq_t result = 2 * binomial(m + n - 2, m - 1);

    /* 2 * (m+n-4) */
    result += 2 * ((seq_t)(m + n - 4));

    /* (m-2) * (n-2) * (m+n-5) */
    result += (seq_t)(m - 2) * (seq_t)(n - 2) * (seq_t)(m + n - 5);

    /* 2 * sum_{i=1}^{m-2} sum_{j=1}^{n-2} ((m-2-i)*(n-2-j)+2) * binomial(i+j,i) */
    for (idx_t i = 1; i <= m - 2; ++i) {
        seq_t mi = (seq_t)(m - 2 - i);
        for (idx_t j = 1; j <= n - 2; ++j) {
            seq_t mj = (seq_t)(n - 2 - j);
            seq_t inner = mi * mj + 2;                       /* ((m-2-i)(n-2-j)+2) */
            seq_t coef  = binomial(i + j, i);               /* binomial(i+j, i) */
            result += 2 * inner * coef;                      /* outer factor 2 */
        }
    }
    return result;
}

/* External function required by the evaluation framework. */
seq_t a(idx_t n)
{
    if (n == 0) { /* offset is 1; return 0 for undefined index 0 */
        return (seq_t)0;
    }

    idx_t s = find_sum_index(n);                      /* antidiagonal sum m+n */
    /* entries before diagonal s: sum of sizes of all previous diagonals */
    idx_t before = (s >= 3) ? ((s - 2) * (s - 1) / 2) : 0;
    idx_t offset = n - before - 1;                    /* 0‑based offset within the diagonal */
    idx_t m = 1 + offset;                             /* m ranges from 1 to s-1 */
    idx_t n2 = s - m;                                 /* corresponding n2 */

    return compute_T(m, n2);
}

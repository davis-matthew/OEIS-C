/*
 * A351776  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) = n! * Sum_{j=0..n} (-k)^(n-j) *
 *          (n-j)^j/j!.
 * https://oeis.org/A351776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   Square array T(n,k) * T(0,k) = 1 * T(n,k) = -k * n *
 *            sum_{j=0}^{n-1} binomial(n-1,j) * T(j,k) (n > 0) * Sequence a(n)
 *            is obtained by reading T(n,k) along anti‑diagonals *
 *            (downwards). For example: a(0)=T(0,0), a(1)=T(0,1), a(2)=T(1,0),
 *            …
 * Range:     i8 n <= 30, i16 n <= 50, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t, NULL */
#include <stdint.h>   /* uint64_t */
#include <stdlib.h>   /* malloc, realloc, abort */
#include <assert.h>   /* assert */

static void antidiagonal_coords(idx_t n, idx_t *row, idx_t *col)
{
    /* Find diagonal d such that n < (d+1)(d+2)/2. */
    idx_t d = 0;
    idx_t cumulative = 1;                 /* (d+1)(d+2)/2 for current d */
    while (n >= cumulative) {
        d++;
        cumulative = (d + 1) * (d + 2) / 2;
    }
    idx_t prev_cumulative = d * (d + 1) / 2;
    idx_t offset = n - prev_cumulative;   /* position on the diagonal */
    *row = offset;                        /* n */
    *col = d - offset;                    /* k */
}

static seq_t compute_T(idx_t n, idx_t k)
{
    /* Compute T(0..n, k) using the recurrence.  Cache grows as required. */
    if (n == 0)
        return (seq_t)1;

    /* Cache of T values for the current column k.  The buffer is resized
     * only when a larger n is requested.  For C++ builds the memory
     * allocation is performed with (seq_t*)realloc, which works for
     * integer types and for small arbitrary‑precision classes that are
     * trivially default‑constructible. */
    static seq_t *T_buf = NULL;
    static idx_t T_cap = 0;

    if (n + 1 > T_cap) {
        idx_t new_cap = n + 1;
        seq_t *new_buf = (seq_t*)realloc(T_buf, (size_t)new_cap * sizeof(seq_t));
        if (!new_buf)
            abort();              /* allocation failure */
        T_buf = new_buf;
        T_cap = new_cap;
    }

    T_buf[0] = (seq_t)1;                /* T(0,k) = 1 */

    for (idx_t m = 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;         /* binomial(m-1,0) */
        for (idx_t j = 0; j < m; ++j) {
            sum += binom * T_buf[j];
            if (j + 1 < m) {
                binom = binom * (seq_t)((m - 1) - j);
                binom = binom / (seq_t)(j + 1);
            }
        }
        T_buf[m] = -(seq_t)k * (seq_t)m * sum;
    }
    return T_buf[n];
}

seq_t a(idx_t n)
{
    idx_t row, col;
    antidiagonal_coords(n, &row, &col);
    return compute_T(row, col);
}

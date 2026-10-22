/*
 * A329431  Irregular triangular array, read by rows: row n shows the
 *          coefficients of the polynomial p(n,x) defined in Comments.
 * https://oeis.org/A329431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   Polynomials p(n,x) defined by * p(0,x) = 1 * p(1,x) = 2 + x *
 *            p(n+1,x) = p(n,x)^2 + 2 * The sequence A329431 is the flattened
 *            coefficients of p(n,x) by rows
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 36, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

struct poly {
    size_t len;   /* number of coefficients (degree + 1) */
    seq_t *coeff; /* coefficients, coeff[k] is coefficient of x^k */
};

#define MAX_ROWS 20          /* enough for n <= 10000 */

static struct poly polys[MAX_ROWS];
static size_t poly_count = 0;             /* number of rows stored so far */
static size_t cum_starts[MAX_ROWS + 1];   /* cum_starts[i] = start index of row i */

/* Ensure all rows up to up_to_row have been generated. */
static void
ensure_polys(idx_t up_to_row)
{
    /* --- init row 0 (constant 1) ------------------------------------- */
    if (poly_count == 0) {
        polys[0].len = 1;
        polys[0].coeff = (seq_t *)malloc(sizeof(seq_t));
        polys[0].coeff[0] = (seq_t)1;
        poly_count = 1;
        cum_starts[0] = 0;          /* start of row 0   */
        cum_starts[1] = 1;          /* start of row 1   */
    }

    /* --- init row 1 (coefficients 2,1) --------------------------------- */
    if (poly_count == 1) {
        polys[1].len = 2;
        polys[1].coeff = (seq_t *)malloc(sizeof(seq_t) * 2);
        polys[1].coeff[0] = (seq_t)2;   /* constant term  */
        polys[1].coeff[1] = (seq_t)1;   /* coefficient of x */
        poly_count = 2;
        cum_starts[2] = cum_starts[1] + polys[1].len; /* start of row 2 */
    }

    /* --- generate subsequent rows -------------------------------------- */
    while ((poly_count <= (size_t)up_to_row) && (poly_count < MAX_ROWS)) {
        size_t   prev_len = polys[poly_count - 1].len;
        seq_t   *prev     = polys[poly_count - 1].coeff;

        /* degree doubles; new length = 2*prev_len - 1 */
        size_t   new_len  = 2 * prev_len - 1;
        seq_t   *new_coeff = (seq_t *)calloc(new_len, sizeof(seq_t));

        /* convolution: (prev * prev) */
        for (size_t i = 0; i < prev_len; ++i) {
            seq_t a = prev[i];
            for (size_t j = 0; j < prev_len; ++j) {
                new_coeff[i + j] += a * prev[j];
            }
        }

        /* add constant 2 */
        new_coeff[0] += (seq_t)2;

        /* store new polynomial */
        polys[poly_count].len = new_len;
        polys[poly_count].coeff = new_coeff;

        /* update cumulative start indices */
        cum_starts[poly_count + 1] = cum_starts[poly_count] + new_len;

        ++poly_count;
    }
}

seq_t a(idx_t n)
{
    /* Ensure we have rows covering the requested index n. */
    while (cum_starts[poly_count] <= (size_t)n) {
        /* Next row to generate is poly_count. */
        ensure_polys((idx_t)poly_count);
    }

    /* Find the row that contains index n. */
    size_t row = 0;
    while ((row + 1 < poly_count) && (cum_starts[row + 1] <= (size_t)n)) {
        ++row;
    }

    /* Offset within the row. */
    size_t offset = (size_t)n - cum_starts[row];
    return polys[row].coeff[offset];
}

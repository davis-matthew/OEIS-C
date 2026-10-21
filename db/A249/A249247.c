/*
 * A249247  Triangular array: row n gives the coefficients of the polynomial
 *          p(n,x) defined in Comments.
 * https://oeis.org/A249247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 25, u32 n <= 55, u64 n <= 131,
 *            u128 n <= 324, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A249247: triangular array of coefficients of the polynomials
   p(n,x) defined by
       p(0)=p(1)=1,
       p(k) = (k‑1)*p(k‑1) + x*p(k‑2)   (k ≥ 2).
   a(n) is the n‑th element in row‑major order of this array. */
#include <stddef.h>

#define MAX_ROWS 200          /* sufficient for n up to 10 000 */
#define MAX_COEF 200          /* maximum number of coefficients in a row */

static seq_t coeff[MAX_ROWS][MAX_COEF];   /* triangle storage   */
static unsigned char row_ok[MAX_ROWS] = {0};    /* flags for computed rows   */
static idx_t max_row_computed = 1;              /* rows 0 and 1 are ready */

/* Length of row r :   floor(r/2)+1   */
static idx_t row_len(idx_t r)
{
    return r / 2 + 1;
}

/* Initialise the two base rows p(0) and p(1) */
static void init_base_rows(void)
{
    if (!row_ok[0]) {
        coeff[0][0] = (seq_t)1;
        row_ok[0] = 1;
    }
    if (!row_ok[1]) {
        coeff[1][0] = (seq_t)1;
        row_ok[1] = 1;
    }
}

/* Compute all rows up to and including r (if not already done) */
static void compute_rows_upto(idx_t r)
{
    if (r <= max_row_computed) return;

    for (idx_t k = max_row_computed + 1; k <= r; ++k) {
        idx_t len_prev  = row_len(k - 1);
        idx_t len_prev2 = row_len(k - 2);          /* k≥2 → fine */
        idx_t len_cur   = row_len(k);
        seq_t mult = (seq_t)(k - 1);

        for (idx_t i = 0; i < len_cur; ++i) {
            seq_t val = 0;
            if (i < len_prev)
                val = val + mult * coeff[k - 1][i];
            if (k >= 2 && i > 0 && (i - 1) < len_prev2)
                val = val + coeff[k - 2][i - 1];
            coeff[k][i] = val;
        }
        row_ok[k] = 1;
    }
    max_row_computed = r;
}

/* Public function required by the template */
seq_t a(idx_t n)
{
    init_base_rows();

    /* Find the row r that contains the n‑th element (row‑major order) */
    idx_t cum = 0;
    idx_t r = 1;
    while (cum + row_len(r) < n) {
        cum += row_len(r);
        ++r;
    }

    compute_rows_upto(r);

    idx_t pos = n - cum - 1;   /* 0‑based position within the row */
    return coeff[r][pos];
}

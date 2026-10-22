/*
 * A355540  Triangle read by rows. Row n gives the coefficients of
 *          Product_{k=0..n} (x - k!) expanded in decreasing powers of x, with
 *          row 0 = {1}.
 * https://oeis.org/A355540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = coefficient T(r,k) in the expansion of * Π_{i=0}^{r-1} (x
 *            - i!) * where r is chosen so that a(n) lies in flatten‑row r *
 *            and k its position in that row (0‑based). The coefficient is *
 *            T(r,k) = (-1)^k * e_k(0!,1!,…, (r-1)!) * with e_k the elementary
 *            symmetric sum of the factorials. This * i...
 * Range:     i8 n <= 16, i16 n <= 23, i32 n <= 39, i64 n <= 44, i128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* Determine which triangle row contains index n.
       Row r has r+1 elements, and the total number of
       elements up to and including row r is (r+1)(r+2)/2. */
    idx_t row = 0;
    idx_t sum = 1;                 /* cumulative count up to current row */
    while (sum <= n) {
        ++row;
        sum += row + 1;            /* add the size of the next row */
    }
    idx_t prev_sum = sum - (row + 1);
    idx_t k = n - prev_sum;        /* position inside the row (0‑based) */

    /* Compute the e_k values for this row.
       r ≤ 141 for n ≤ 10 000, so a fixed array is safe. */
    int R = (int)row;
    seq_t e[200] = {0};
    e[0] = 1;                      /* e_0 = 1 by definition */

    seq_t fact = 1;                 /* factorial of current index i */
    for (int i = 0; i < R; ++i) {
        seq_t f = fact;             /* f = i! */

        /* Update e from high to low to avoid overwriting needed values. */
        for (int j = i + 1; j >= 1; --j) {
            e[j] = e[j] + f * e[j - 1];
        }

        /* Prepare factorial for next iteration: (i+1)! = i! * (i+1) */
        fact = fact * (i + 1);
    }

    seq_t coeff = e[(int)k];
    if (k & 1) coeff = -coeff;     /* apply the (-1)^k sign */
    return coeff;
}

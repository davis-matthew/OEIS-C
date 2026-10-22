/*
 * A337301  Triangle read by rows in which row n lists the closest integers to
 *          diagonal lengths of regular n-gon with unit edge length, n >= 4.
 * https://oeis.org/A337301
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the nth element (offset 4) of the triangle * T(n,k) =
 *            round(sin((k+1)*Pi/n)/sin(Pi/n)), n ≥ 4, 1 ≤ k ≤ n‑3, * read row
 *            by row. This implementation maps the overall index * back to
 *            (row, column) and evaluates the ratio with double * precision;
 *            the result is rounded to the nearest integer
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

/* External linkage as required */
seq_t a(idx_t n)
{
    if (n < 4)
        return 0;  /* Undefined for indices below the offset */

    /* Translate index to row and column of the triangle */
    idx_t m = n - 4;                      /* zero‑based offset */
    idx_t val = 1 + 8 * m;                /* argument for sqrt   */
    idx_t s = (idx_t)floor(sqrt((double)val)); /* floor sqrt(1+8m)  */
    idx_t x = (s - 1) / 2;                /* floor((-1+sqrt)/2) */
    idx_t r = x + 4;                      /* row number (n‑type) */
    idx_t cumulative_before = x * (x + 1) / 2; /* terms before this row */
    idx_t k = m - cumulative_before + 1; /* column within the row (1‑based) */

    /* Evaluate the sine ratio and round to nearest integer */
    static const double PI = 3.14159265358979323846;
    double sinBase = sin(PI / (double)r);
    double ratio = sin(((k + 1) * PI) / (double)r) / sinBase;
    int value = (int)floor(ratio + 0.5);   /* round to nearest integer */

    return (seq_t)value;
}

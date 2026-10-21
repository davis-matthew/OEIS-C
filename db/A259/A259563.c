/*
 * A259563  a(n) = 81*n^3*(n-1)^5*(n+1)^2*(n^2-6*n+1)*(n^3-79*n^2+163*n-81).
 * https://oeis.org/A259563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 81*n^3*(n-1)^5*(n+1)^2*(n^2-6*n+1)*(n^3-79*n^2+163*n-81)
 * Range:     i8 n <= 1, i16 n <= 1, i32 n <= 2, i64 n <= 13, i128 n <= 270,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;
    seq_t sn = (seq_t)n;
    seq_t n1 = sn - 1;
    seq_t n2 = sn + 1;

    /* compute powers */
    seq_t n3      = sn * sn * sn;          /* n^3                               */
    seq_t n1_5    = n1 * n1 * n1 * n1 * n1;/* (n-1)^5                           */
    seq_t n2_2    = n2 * n2;                /* (n+1)^2                           */

    /* remaining polynomial factors */
    seq_t t1 = sn * sn - (seq_t)6 * sn + 1; /* n^2 - 6n + 1                      */
    seq_t t2 = n3 - (seq_t)79 * (sn * sn) + (seq_t)163 * sn - 81; /* n^3-79n^2+163n-81 */

    /* multiply in an order that keeps 64‑bit intermediates within range */
    seq_t res = (seq_t)81;
    res *= n3;
    res *= n1_5;
    res *= n2_2;
    res *= t1;
    res *= t2;
    return res;
}

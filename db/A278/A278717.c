/*
 * A278717  Triangle read by rows: T(n, m) gives the difference between the
 *          even and odd leg of the primitive Pythagorean triangle determined
 *          by (n, m) with n > m >= 1, gcd(n, m) = 1 and n+m odd, or 0 for
 *          other (n, m).
 * https://oeis.org/A278717
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) = T(r, m) where r and m are obtained from n by reading the
 *            triangle of T(n, m) values row‑wise starting with a(2) = T(2,1).
 *            T(r, m) is defined by T(r, m) = 2*r*m - (r^2 - m^2) if r > m ≥
 *            1, gcd(r, m)=1, r+m is odd; 0 otherwise
 * Range:     i8 n <= 45, i16 n <= 81, i32 n <= 81, i64 n <= 81, i128 n <= 81,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    /*  Offset 2: the first term corresponds to n = 2  */
    if (n < 2) return 0;

    idx_t q = n - 1;                      /* 0‑based term index after offset  */
    idx_t r = 2;                          /* start from the first row */

    /* find the row r such that C(r) = (r-1)*r/2 ≥ q  */
    while (1) {
        seq_t cr = ((seq_t)(r - 1) * (seq_t)r) / 2;
        if (cr >= (seq_t)q) break;
        ++r;
    }

    /* m is the column index within the row */
    seq_t cp = ((seq_t)(r - 2) * (seq_t)(r - 1)) / 2;   /* C(r‑1) */
    idx_t m = (idx_t)((seq_t)q - cp);                  /* 1 ≤ m ≤ r‑1 */

    /* apply the conditions of the definition */
    if (m == 0) return 0;                 /* safeguard, shouldn't happen */
    if (((r + m) & 1) == 0) return 0;     /* r + m must be odd      */
    if (gcd((seq_t)r, (seq_t)m) != 1) return 0;  /* coprime requirement   */

    /* compute T(r, m) = 2*r*m - (r^2 - m^2) */
    seq_t val = 2 * (seq_t)r * (seq_t)m
                - ((seq_t)r * (seq_t)r - (seq_t)m * (seq_t)m);
    return val;
}

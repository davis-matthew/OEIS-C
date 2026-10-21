/*
 * A254749  1-gonal pyramidal numbers.
 * https://oeis.org/A254749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n*(4 + 3*n - n^2)/6
 * Range:     i8 n <= 10, i16 n <= 59, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 1 the offset says behavior does not matter.
       We simply return 0 to avoid any undefined arithmetic. */
    if (n == 0) return (seq_t)0;

    /* Use 64‑bit intermediate arithmetic to avoid overflow when
       seq_t is a small native integer type. The intermediate result
       fits in long long for all n up to the 10000‑limit of this
       implementation. */
    long long nn   = (long long)n;
    long long temp = 4 + 3 * nn - nn * nn;      /* 4 + 3n - n²  */
    long long num  = nn * temp;                 /* n * (4 + 3n - n²) */

    return (seq_t)(num / 6);
}

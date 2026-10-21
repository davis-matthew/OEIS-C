/*
 * A269554  Expansion of (3*x^2 + 244*x + 1)/(x^3 - 99*x^2 + 99*x - 1).
 * https://oeis.org/A269554
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 0, i16 n <= 1, i32 n <= 4, i64 n <= 9, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = -1, a(1) = -343, a(2) = -33861;
   a(n) = 99*a(n-1) - 99*a(n-2) + a(n-3) for n >= 3. */
#include <stddef.h>

seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)-1;
    if (n == (idx_t)1) return (seq_t)-343;
    if (n == (idx_t)2) return (seq_t)-33861;

    seq_t a0 = (seq_t)-1;   /* a(n-3) */
    seq_t a1 = (seq_t)-343; /* a(n-2) */
    seq_t a2 = (seq_t)-33861; /* a(n-1) */
    seq_t an;

    for (idx_t i = (idx_t)3; i <= n; ++i) {
        an = (seq_t)99 * a2 - (seq_t)99 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}

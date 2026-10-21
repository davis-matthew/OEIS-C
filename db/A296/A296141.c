/*
 * A296141  Sum of the smaller parts of the partitions of n into two distinct
 *          parts with the larger part even.
 * https://oeis.org/A296141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A296141: sum of the smaller parts of partitions of n into two
 * distinct parts where the larger part is even.
 *
 * Formula: a(n) = floor((n+1)/4)^2 * (n mod 2) + (1 + floor((n-2)/4)) *
 *              floor((n-2)/4) * ((n+1) mod 2)
 * This reduces to:
 *   if n is odd: a(n) = ((n+1)/4)^2
 *   if n is even: a(n) = ((n-2)/4) * ((n-2)/4 + 1)
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* n is 1‑based offset.  For safety, handle n == 0 as 0. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Always use seq_t arithmetic for the result. */
    if (n & 1) {                       /* n is odd */
        seq_t k = ((seq_t)n + 1) / 4;   /* floor((n+1)/4) */
        return k * k;
    } else {                           /* n is even */
        /* For even n, n >= 2. */
        seq_t k = ((seq_t)n - 2) / 4;   /* floor((n-2)/4) */
        return k * (k + 1);
    }
}

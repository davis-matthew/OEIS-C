/*
 * A303500  The smallest positive even integer that can be written with n
 *          digits in base 3/2.
 * https://oeis.org/A303500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A303500:
   a(0) = 2
   For n > 0, let
        T = (3 * a(n-1) / 2)
        d = T % 2   (so d is 0 if T is even, 1 if T is odd)
   Then
        a(n) = a(n-1) * 10 + d
   The values a(n) are stored as decimal integers of the base‑3/2
   representation of the smallest even integer with (n+1) digits. */
seq_t a(idx_t n) {
    seq_t value = 2;      /* smallest even integer (decimal) */
    seq_t rep   = 2;      /* decimal representation of digits */

    for (idx_t i = 0; i < n; ++i) {
        /* next integer value: V = (3*value)/2 + d, d is 0 or 1 */
        seq_t t = (value * (seq_t)3) / (seq_t)2;  /* (3*value)/2, value is always even */
        seq_t d = ((t & (seq_t)1) != 0) ? (seq_t)1 : (seq_t)0;
        value = t + d;
        rep   = rep * (seq_t)10 + d;
    }

    return rep;
}

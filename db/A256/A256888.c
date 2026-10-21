/*
 * A256888  Continued fraction expansion of 1 + sqrt(64 / 37).
 * https://oeis.org/A256888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Periodic sequence: a(n) = a(n-10)
   a(0) = 2, a(1) = 3, a(2) = 5, a(3) = 1, a(4) = 3,
   a(5) = 1, a(6) = 3, a(7) = 1, a(8) = 5, a(9) = 3 */
seq_t a(idx_t n)
{
    static const seq_t period[10] = {2, 3, 5, 1, 3, 1, 3, 1, 5, 3};
    return period[n % 10];
}

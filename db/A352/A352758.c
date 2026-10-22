/*
 * A352758  a(n) = (3*(2*n - 1)^2*((2*n - 1)^2 + 2) - 2*n + 3)/2 for n > 0.
 * https://oeis.org/A352758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*(2*n - 1)^2*((2*n - 1)^2 + 2) - 2*n + 3)/2
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 115, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute t = 2*n - 1 (as seq_t). */
    seq_t t = (seq_t)n * 2 - 1;
    seq_t t2 = t * t;                      /* t^2 */
    seq_t num = 3 * t2 * (t2 + 2)          /* 3*t^2*(t^2+2) */
              - (seq_t)n * 2 + 3;          /* -2*n + 3 */
    return num / 2;
}

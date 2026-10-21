/*
 * A245346  Sum of digits of n in fractional base 10/3.
 * https://oeis.org/A245346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A007953(A024658(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t x = n;
    while (x) {
        idx_t d = x % 10;      /* least‑significant digit in base 10/3 */
        sum += (seq_t)d;
        x = (x / 10) * 3;      /* next integer to process */
    }
    return sum;
}

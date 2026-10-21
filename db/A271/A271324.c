/*
 * A271324  a(n) = n + floor(n/4) + (n mod 4).
 * https://oeis.org/A271324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + ⌊n/4⌋ + (n mod 4)
 * Range:     u8 n <= 202, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;      /* promote index to seq_t */
    seq_t k = N / 4;         /* ⌊n/4⌋  */
    seq_t r = N % 4;         /* n mod 4 */
    return k * 5 + r * 2;    /* 5*⌊n/4⌋ + 2*(n mod 4) = a(n) */
}

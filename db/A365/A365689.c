/*
 * A365689  Final decimal digit of n^((n+1)^(n+2)) = A030198(n).
 * https://oeis.org/A365689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n^((n+1)^(n+2)) mod 10 – this sequence is periodic with
 *            period 10: [0, 1, 2, 1, 4, 5, 6, 1, 8, 1]
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[10] = {
        0, 1, 2, 1, 4, 5, 6, 1, 8, 1
    };
    return period[n % 10];
}

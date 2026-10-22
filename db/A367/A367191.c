/*
 * A367191  a(n) is the largest positive number k such that k^2 + k*n + n^2 is
 *          a perfect square, or 0 if no such k exists.
 * https://oeis.org/A367191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0. For n > 0 let q = n / 4. if n % 4 == 0: a(n) = (q - 1)
 *            * (3 * q + 1); if n % 4 == 1: a(n) = 4 * q * (3 * q + 1); if n %
 *            4 == 2: a(n) = 2 * q * (3 * q + 2); if n % 4 == 3: a(n) = (2 * q
 *            + 1) * (6 * q + 5);
 * Range:     u8 n <= 18, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    idx_t q = n / 4;
    switch (n % 4) {
    case 0:
        return (seq_t)(q - 1) * (seq_t)(3 * q + 1);
    case 1:
        return (seq_t)4 * (seq_t)q * (seq_t)(3 * q + 1);
    case 2:
        return (seq_t)2 * (seq_t)q * (seq_t)(3 * q + 2);
    case 3:
        return (seq_t)(2 * q + 1) * (seq_t)(6 * q + 5);
    default: /* unreachable */
        return 0;
    }
}

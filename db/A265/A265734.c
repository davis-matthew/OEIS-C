/*
 * A265734  Permutation of nonnegative integers: a(n) = n + floor(n/5)*(-1)^(n
 *          mod 5).
 * https://oeis.org/A265734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(n/5)*(-1)^(n mod 5)
 * Range:     u8 n <= 213, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n % 5;
    seq_t k = (seq_t)n / 5;
    seq_t coeff = (r % 2 == 0) ? (seq_t)6 : (seq_t)4;   /* 6 for even r, 4 for odd r */
    return coeff * k + (seq_t)r;
}

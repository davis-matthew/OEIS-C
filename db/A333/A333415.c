/*
 * A333415  Odd positive integers in base 2 read backwards.
 * https://oeis.org/A333415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = decimal number whose decimal digits are the binary *
 *            representation of the nth odd positive integer reversed. That
 *            is, * a(n) = reverse_bits(2*n-1) interpreted as a decimal
 *            integer (digits 0/1). * Example: n = 6 → 2*n-1 = 11 (binary
 *            1011) → reversed bits 1101 → decimal 1101
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 512, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the nth odd integer as a seq_t: 2*n-1 */
    seq_t odd = ((seq_t)n << 1) - 1;

    /* Build the decimal number whose digits are the reversed bits of odd. */
    seq_t rev_dec = 0;
    while (odd) {
        rev_dec = rev_dec * 10 + (odd & 1);
        odd >>= 1;
    }
    return rev_dec;
}

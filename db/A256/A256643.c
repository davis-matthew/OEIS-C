/*
 * A256643  a(n) = B*C*(n mod A) + 2*A*C*(n mod B) + 3*A*B*(n mod C) with A=3,
 *          B=5, C=11.
 * https://oeis.org/A256643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 55*(n mod 3) + 66*(n mod 5) + 45*(n mod 11)
 * Range:     u8 n <= 1, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = ((seq_t)55) * ((seq_t)(n % 3)) +
                ((seq_t)66) * ((seq_t)(n % 5)) +
                ((seq_t)45) * ((seq_t)(n % 11));
    return res;
}

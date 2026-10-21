/*
 * A256668  a(n) = 3*B*C*(n mod A) + 5*A*C*(n mod B) + 2*A*B*(n mod C) with
 *          A=7, B=11, C=17.
 * https://oeis.org/A256668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*11*17*(n mod 7) + 5*7*17*(n mod 11) + 2*7*11*(n mod 17)
 * Range:     u8 none, u16 n <= 1309, u32 n <= 1309, u64 n <= 1309,
 *            u128 n <= 1309, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          a(n) = 561*(n mod 7) + 595*(n mod 11) + 154*(n mod 17) */

seq_t a(idx_t n)
{
    /* Compute the remainders of n modulo 7, 11, and 17 */
    seq_t r7 = (seq_t)(n % 7);
    seq_t r11 = (seq_t)(n % 11);
    seq_t r17 = (seq_t)(n % 17);

    /* Combine with the predetermined coefficients */
    return 561 * r7 + 595 * r11 + 154 * r17;
}

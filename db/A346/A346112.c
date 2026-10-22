/*
 * A346112  Size of the smallest regular polygon chain for a regular polygon
 *          with n sides.
 * https://oeis.org/A346112
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, more, hear
 * Formula:   a(n) = 3 if n ≡ 0 (mod 6); * 4 if n ≡ 4 or 8 (mod 12); * 5 if n
 *            == 10; * 4 if n == 14; * 6 otherwise
 * Range:     u8 n <= 20, u16 n <= 20, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle modulus and special cases as described above. */
    if (n % 6 == 0)
        return (seq_t)3;

    if ((n % 12 == 4) || (n % 12 == 8))
        return (seq_t)4;

    if (n == 10)
        return (seq_t)5;

    if (n == 14)
        return (seq_t)4;

    return (seq_t)6;
}

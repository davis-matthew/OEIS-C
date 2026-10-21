/*
 * A246508  Digital root of numbers congruent to {1,7,11,13,17,19,23,29}
 *          modulo 30.
 * https://oeis.org/A246508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = digital_root of the n-th integer congruent to
 *            {1,7,11,13,17,19,23,29} modulo 30. It equals A010888(A007775(n))
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Be defensive for n == 0, though the sequence is defined
       for n >= 1.  The result is 0, which matches the
       digital root of 0. */
    if (n == 0)
        return 0;

    idx_t n_minus1 = n - 1;
    /* r = (n-1) mod 8 determines which offset to use. */
    idx_t r = n_minus1 % 8;

    /* m = floor((n-1)/8).  We only need m modulo 3 because
       30 ≡ 3 (mod 9) and the digital root depends on
       number mod 9.  Thus we compute m_div3 = m % 3. */
    idx_t m_div3 = (n_minus1 / 8) % 3;

    static const seq_t offsets[8] = {1, 7, 11, 13, 17, 19, 23, 29};

    seq_t offset = offsets[r];
    seq_t offset_mod9 = offset % 9;

    /* number_mod9 = (3 * m_div3 + offset_mod9) mod 9 */
    seq_t number_mod9 = ((seq_t)3 * (seq_t)m_div3 + offset_mod9) % 9;

    /* For positive numbers, the digital root is 9 when the
       remainder is 0, otherwise it is the remainder itself. */
    return (number_mod9 == 0 ? 9 : number_mod9);
}

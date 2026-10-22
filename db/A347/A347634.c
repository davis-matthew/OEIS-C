/*
 * A347634  Number of minimum dominating sets in the n-prism graph (for n > 3
 *          except n = 6).
 * https://oeis.org/A347634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4 if n ≡ 0 (mod 4), a(n) = 2*n if n is odd (1 or 3 mod
 *            4), a(n) = n*(n+2) if n ≡ 2 (mod 4)
 * Range:     u8 n <= 17, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle n = 0 just in case – although the sequence is defined
       for n ≥ 1, the formula for a(0) is given in the OEIS entry. */
    if (n == 0)
        return (seq_t)4;

    /* n modulo 4 can be obtained with bitwise AND since 4 is a power
       of 2. */
    if ((n & 3) == 0)
        return (seq_t)4;

    /* If n is odd, a(n) = 2*n. */
    if ((n & 1) == 1)
        return (seq_t)2 * (seq_t)n;

    /* Remaining case: n is even but not divisible by 4 → n ≡ 2 (mod 4). */
    seq_t sn = (seq_t)n;
    return sn * (sn + (seq_t)2);
}

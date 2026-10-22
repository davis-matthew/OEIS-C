/*
 * A364447  Repeat [1,2,1,3].
 * https://oeis.org/A364447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 1 if n mod 4 == 1 or 3, 2 if n mod 4 == 2, 3 if n mod 4
 *            == 0 (period 4)
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n modulo 4 using bitwise AND (since 4 is a power of two) */
    unsigned r = (unsigned)(n & 3);   /* r is 0,1,2,3 */

    /* Return the periodic term based on the remainder */
    switch (r)
    {
        case 1:  return (seq_t)1;  /* n ≡ 1 (mod 4) */
        case 2:  return (seq_t)2;  /* n ≡ 2 (mod 4) */
        case 3:  return (seq_t)1;  /* n ≡ 3 (mod 4) */
        case 0:  return (seq_t)3;  /* n ≡ 0 (mod 4) */
        default: return (seq_t)0;  /* unreachable for unsigned r < 4 */
    }
}

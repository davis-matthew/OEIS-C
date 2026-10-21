/*
 * A271572  a(n) = n^7 mod 32.
 * https://oeis.org/A271572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 mod 32
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use the fact that n^7 mod 32 = (n mod 32)^7 mod 32.
       Compute using only the low 5 bits to avoid overflow.  */
    static const seq_t MASK = (seq_t)31;          /* 2^5 - 1  */
    seq_t m = (seq_t)n & MASK;                   /* n mod 32   */
    seq_t r = m;                                 /* m^1        */
    for (int i = 1; i < 7; ++i)                  /* exponentiation: multiply 6 more times */
    {
        r = (r * m) & MASK;                      /* keep only lowest 5 bits */
    }
    return r;                                    /* result is n^7 mod 32 */
}

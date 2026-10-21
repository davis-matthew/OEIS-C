/*
 * A270033  a(n) is the smallest b for which the base-b representation of n
 *          contains at least one 7 (or 0 if no such base exists).
 * https://oeis.org/A270033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the smallest base b (b≥8) such that the base‑b
 *            representation of n contains at least one digit 7; return 0 if
 *            no such base exists. This is equivalent to searching all bases
 *            from 8 up to n (or n+1 for n<8) and testing the digits
 * Range:     u8 n <= 808, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle trivial cases first */
    if (n < 1) return (seq_t)0;
    if (n <= 6) return (seq_t)0;   /* numbers 1–6 have no digit 7 in any base */
    if (n == 7) return (seq_t)8;   /* base 8 represents 7 as the single digit 7 */

    /* For n ≥ 8, search all bases b = 8 … n */
    for (idx_t b = 8; b <= n; ++b)
    {
        seq_t tmp = (seq_t)n;
        while (tmp)
        {
            if (tmp % (seq_t)b == (seq_t)7)
                return (seq_t)b;     /* smallest base found */
            tmp /= (seq_t)b;
        }
    }

    /* No base found that contains the digit 7 */
    return (seq_t)0;
}

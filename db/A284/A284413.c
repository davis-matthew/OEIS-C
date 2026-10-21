/*
 * A284413  Exponent of 3 in 2^n + 1.
 * https://oeis.org/A284413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = 0 if n is even; otherwise 1 + v_3(n) where v_3(n) is the
 *            exponent of 3 in n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Odd n: a(n) = 1 + exponent of 3 dividing n; even n: 0. */
    if ((n & 1) == 0)                       /* n even */
        return 0;

    idx_t tmp = n;                          /* copy n to preserve original value */
    seq_t ex  = 0;                          /* exponent of 3 in n */

    while (tmp % 3 == 0)                    /* divide out factors of 3 */
    {
        tmp /= 3;
        ex += 1;
    }

    return ex + 1;                          /* add the 1 from LTE for 2^n + 1 */
}

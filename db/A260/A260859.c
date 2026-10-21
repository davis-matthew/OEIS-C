/*
 * A260859  Base-9 representation of a(n) is the concatenation of the base-9
 *          representations of 1, 2, ..., n, n-1, ..., 1.
 * https://oeis.org/A260859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of the base‑9 representations of 1, 2, …,
 *            n, n‑1, …, 1, interpreted as a base‑9 number
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    seq_t result = 0;
    /* iterate through 1 … n */
    for (idx_t i = 1; i <= n; ++i)
    {
        /* number of base‑9 digits of i */
        unsigned d = 1;
        idx_t tmp = i;
        while (tmp >= 9U)
        {
            tmp /= 9U;
            d++;
        }
        /* shift left by d digits in base 9 */
        for (unsigned k = 0; k < d; ++k)
            result *= (seq_t)9;
        result += (seq_t)i;
    }
    /* iterate through n‑1 … 1 */
    for (idx_t i = n - 1; n > 0 && i >= 1; --i)
    {
        unsigned d = 1;
        idx_t tmp = i;
        while (tmp >= 9U)
        {
            tmp /= 9U;
            d++;
        }
        for (unsigned k = 0; k < d; ++k)
            result *= (seq_t)9;
        result += (seq_t)i;
    }
    return result;
}

/*
 * A337842  The smallest palindrome with exactly n circular loops (or holes)
 *          in its decimal representation.
 * https://oeis.org/A337842
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0) = 1; a(2*m) = 8 repeated m times; a(4*m + 1) = 8 repeated m
 *            times, then 0, then 8 repeated m times; a(4*m + 3) = 6, followed
 *            by 8 repeated m times, then 0, then 8 repeated m times, then 6
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 18, u64 n <= 38, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t result = 0;
    idx_t k;

    if ((n & 1) == 0) {                       /* n even: a(2*m) */
        k = n >> 1;                           /* k = m */
        for (idx_t i = 0; i < k; ++i)
            result = result * 10 + 8;
    } else if ((n & 3) == 1) {                /* n ≡ 1 (mod 4): a(4*m+1) */
        k = (n - 1) / 4;                      /* k = m */
        for (idx_t i = 0; i < k; ++i)
            result = result * 10 + 8;
        result = result * 10 + 0;
        for (idx_t i = 0; i < k; ++i)
            result = result * 10 + 8;
    } else {                                  /* n ≡ 3 (mod 4): a(4*m+3) */
        k = (n - 3) / 4;                      /* k = m */
        result = 6;
        for (idx_t i = 0; i < k; ++i)
            result = result * 10 + 8;
        result = result * 10 + 0;
        for (idx_t i = 0; i < k; ++i)
            result = result * 10 + 8;
        result = result * 10 + 6;
    }

    return result;
}

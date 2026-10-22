/*
 * A362117  Concatenation of first n numbers in base 5.
 * https://oeis.org/A362117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of base‑5 representations of 1…n
 *            interpreted as decimal digits
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)0;
    for (idx_t i = 1; i <= n; ++i)
    {
        idx_t temp = i;
        unsigned char digits[32];
        unsigned int sz = 0;
        /* convert i to base-5, least significant digit first */
        do {
            digits[sz++] = (unsigned char)(temp % 5);
            temp /= 5;
        } while (temp > 0);
        /* append digits in reverse order (most significant first) */
        for (unsigned int j = sz; j > 0; --j) {
            cur = cur * (seq_t)10 + (seq_t)digits[j-1];
        }
    }
    return cur;
}

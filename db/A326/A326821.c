/*
 * A326821  a(n) is the number of positive numbers k < n such that n+k and n-k
 *          have no digit in common.
 * https://oeis.org/A326821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of k (1 <= k < n) such that n + k and n - k have
 *            no common decimal digit
 * Range:     u8 n <= 797, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t k = 1; k < n; ++k)
    {
        seq_t sum = (seq_t)n + (seq_t)k;
        seq_t diff = (seq_t)n - (seq_t)k;

        uint16_t mask_sum = 0;
        uint16_t mask_diff = 0;

        seq_t x = sum;
        if (x == 0)
            mask_sum = 1 << 0;
        else
            while (x > 0) {
                uint8_t d = (uint8_t)(x % 10);
                mask_sum |= 1u << d;
                x /= 10;
            }

        x = diff;
        if (x == 0)
            mask_diff = 1 << 0;
        else
            while (x > 0) {
                uint8_t d = (uint8_t)(x % 10);
                mask_diff |= 1u << d;
                x /= 10;
            }

        if ((mask_sum & mask_diff) == 0)
            ++count;
    }
    return count;
}

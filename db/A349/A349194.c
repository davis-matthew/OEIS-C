/*
 * A349194  a(n) is the product of the sum of the first i digits of n, as i
 *          goes from 1 to the total number of digits of n.
 * https://oeis.org/A349194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = Π_{i=1}^{len(n)} (Σ_{j=1}^i digit_j(n))
 * Range:     u8 n <= 267, u16 n <= 6988, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where digit_1 is the most significant digit */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* out of spec; n≥1 */

    /* digits[0] = least–significant digit, digits[d-1] = most–significant */
    unsigned char digits[20];
    int d = 0;
    idx_t tmp = n;
    while (tmp) {
        digits[d++] = tmp % 10;
        tmp /= 10;
    }

    seq_t sum = (seq_t)0;
    seq_t prod = (seq_t)1;
    for (int i = d - 1; i >= 0; --i) {
        sum = sum + (seq_t)digits[i];
        prod = prod * sum;
    }
    return prod;
}

/*
 * A351985  If n = abcd... in decimal, a(n) = |a^3 - b^3 + c^3 - d^3 + ...|.
 * https://oeis.org/A351985
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = |Σ_{i=0}^{k-1} (-1)^i * d_i^3| where n has decimal digits
 *            d_0d_1...d_{k-1}
 * Range:     u8 n <= 6, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t temp = n;
    seq_t sum_pos = 0;
    seq_t sum_neg = 0;
    int sign = 1;

    /* Handle n = 0 explicitly */
    if (temp == 0)
        return 0;

    while (temp > 0)
    {
        unsigned digit = temp % 10;
        seq_t cube = (seq_t)digit * (seq_t)digit * (seq_t)digit;

        if (sign > 0)
            sum_pos += cube;
        else
            sum_neg += cube;

        sign = -sign;
        temp /= 10;
    }

    return (sum_pos >= sum_neg) ? (sum_pos - sum_neg) : (sum_neg - sum_pos);
}

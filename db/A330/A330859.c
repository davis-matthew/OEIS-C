/*
 * A330859  The additive version of the 'Decade transform' : to obtain a(n)
 *          write n as a sum of its power-of-ten parts and then continue to
 *          calculate the sum of the adjacent parts until a single number
 *          remains.
 * https://oeis.org/A330859
 *
 * Offset:    100    (a(100) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{k=0..m} d_k*C(m,k)*10^k
 * Range:     u8 n <= 162, u16 n <= 162, u32 n <= 162, u64 n <= 162,
 *            u128 n <= 162, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    // Get decimal digits of n (least significant digit first)
    unsigned char digits[64];
    idx_t temp = n;
    int m = 0;

    if (temp == 0) {
        digits[0] = 0;
        m = 0;
    } else {
        digits[0] = (unsigned char)(temp % 10);
        temp /= 10;
        while (temp > 0) {
            digits[++m] = (unsigned char)(temp % 10);
            temp /= 10;
        }
    }

    seq_t result = 0;
    seq_t binom   = 1;  // C(m,0)
    seq_t pow10   = 1;  // 10^0

    for (int k = 0; k <= m; ++k) {
        result += (seq_t)digits[k] * binom * pow10;
        if (k < m) {
            pow10 *= 10;
            binom = binom * (seq_t)(m - k) / (seq_t)(k + 1);
        }
    }

    return result;
}

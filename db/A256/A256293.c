/*
 * A256293  Apply the transformation 0 -> 1 -> 2 -> 0 to the digits of n
 *          written in base 3, then convert back to base 10.
 * https://oeis.org/A256293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Σ ( (digit_i(n)+1) mod 3 ) * 3^{k-i} where digit_i(n) are
 *            the base‑3 digits of n (from most significant to least). This is
 *            equivalent to transforming each trinary digit by 0→1, 1→2, 2→0
 *            and interpreting the result as a base‑3 number
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* small buffer for base‑3 digits; bound: log_3(2^64) < 41 */
    int digits[64];
    int len = 0;
    idx_t t = n;

    if (t == 0) {
        digits[len++] = 0;
    } else {
        while (t > 0) {
            digits[len++] = t % 3;
            t /= 3;
        }
    }

    seq_t res = 0;
    for (int i = len - 1; i >= 0; --i) {
        int d = digits[i];
        int td = (d + 1) % 3;            /* transformed digit */
        res = res * 3 + td;
    }
    return res;
}

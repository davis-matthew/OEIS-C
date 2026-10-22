/*
 * A384212  a(n) is the number of bases >= 2 in which the alternating sum of
 *          digits of n is equal to 0.
 * https://oeis.org/A384212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A384212:  
   a(n) = number of bases b ≥ 2 such that the alternating sum of the
   base‑b digits of n equals 0.  */
seq_t a(idx_t n)
{
    /* a(1) and a(2) are 0 by definition.  */
    if (n < 3) return 0;

    seq_t count = 0;

    /* Consider all bases b with 2 ≤ b ≤ n-1.  */
    for (idx_t b = 2; b <= n - 1; ++b) {
        idx_t tmp = n;
        /* Digits in base b (least‑significant first).  uint16_t is enough
           because b ≤ n-1 ≤ 2^64-2 and the largest digit is b-1.  */
        uint16_t digits[64];
        int len = 0;

        /* Obtain the digits.  */
        while (tmp > 0) {
            digits[len++] = (uint16_t)(tmp % b);
            tmp /= b;
        }

        /* Compute the alternating sum starting with + on the most
           significant digit.  */
        int sum  = 0;
        int sign = 1;
        for (int i = len - 1; i >= 0; --i) {
            sum += sign * (int)digits[i];
            sign = -sign;
        }

        if (sum == 0)
            ++count;
    }

    return count;
}

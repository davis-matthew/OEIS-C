/*
 * A364100  Sum of divisors of 5*n-1 of form 5*k+4.
 * https://oeis.org/A364100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of divisors of (5*n-1) that are ≡ 4 (mod 5)
 * Range:     u8 n <= 46, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* compute m = 5*n - 1 */
    idx_t m = 5 * n - 1;

    seq_t sum = 0;
    for (idx_t i = 1; i * i <= m; ++i) {
        if (m % i == 0) {
            idx_t div1 = i;
            idx_t div2 = m / i;

            /* divisor pair handling */
            if (div1 % 5 == 4) {
                sum += (seq_t)div1;
            }
            if (div1 != div2 && div2 % 5 == 4) {
                sum += (seq_t)div2;
            }
        }
    }

    return sum;
}

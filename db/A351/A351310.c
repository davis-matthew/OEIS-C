/*
 * A351310  Sum of the 5th powers of the square divisors of n.
 * https://oeis.org/A351310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d^2 | n} (d^2)^5 = Sum_{d^2 | n} d^10
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 99, u64 n <= 7224,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t result = 1;
    idx_t temp = n;

    /* handle factor 2 */
    idx_t count2 = 0;
    while ((temp & 1) == 0) {
        temp >>= 1;
        ++count2;
    }
    if (count2 > 0) {
        seq_t term = 1;
        seq_t power = 1;
        seq_t p10 = 1;
        /* compute 2^10 */
        for (idx_t j = 0; j < 10; ++j) p10 *= (seq_t)2;
        idx_t m = count2 >> 1;           /* floor(count2/2) */
        for (idx_t i = 1; i <= m; ++i) {
            power *= p10;
            term += power;
        }
        result *= term;
    }

    /* odd prime factors */
    for (idx_t i = 3; i * i <= temp; i += 2) {
        idx_t cnt = 0;
        while (temp % i == 0) {
            temp /= i;
            ++cnt;
        }
        if (cnt > 0) {
            seq_t term = 1;
            seq_t power = 1;
            seq_t p10 = 1;
            /* compute i^10 */
            for (idx_t j = 0; j < 10; ++j) p10 *= (seq_t)i;
            idx_t m = cnt >> 1;           /* floor(cnt/2) */
            for (idx_t k = 1; k <= m; ++k) {
                power *= p10;
                term += power;
            }
            result *= term;
        }
    }

    /* remaining prime factor (if any) has exponent 1 → contributes 1 */
    return result;
}

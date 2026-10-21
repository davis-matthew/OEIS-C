/*
 * A297794  a(n) = Sum_{d|n} min(d, n/d)^4.
 * https://oeis.org/A297794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} min(d, n/d)^4
 * Range:     u8 n <= 15, u16 n <= 179, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* offset: a(1) = 1, so n >= 1 is assumed; n == 0 is handled for safety */
    if (n == 0) {
        return 0;
    }

    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t d2 = n / d;          /* complementary divisor */
            seq_t m = (seq_t)d;         /* cast to the value type */
            seq_t m2 = m * m;           /* m^2 */
            seq_t m4 = m2 * m2;         /* m^4 */
            sum += m4;                  /* add min(d, n/d)^4 once */
            if (d != d2) {              /* add the pair only if distinct */
                sum += m4;
            }
        }
    }
    return sum;
}

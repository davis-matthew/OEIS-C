/*
 * A367493  a(n) = Sum_{d|n} (n-d)^n.
 * https://oeis.org/A367493
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (n-d)^n
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_n(seq_t base, idx_t exp)
{
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i)
        res *= base;
    return res;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            /* divisor d = i */
            idx_t d1 = i;
            seq_t base1 = (seq_t)(n - d1);
            sum += pow_n(base1, n);

            /* counterpart divisor d = n / i */
            idx_t d2 = n / i;
            if (d2 != d1) {
                seq_t base2 = (seq_t)(n - d2);
                sum += pow_n(base2, n);
            }
        }
    }
    return sum;
}

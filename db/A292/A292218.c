/*
 * A292218  The n-th iteration of (k -> k + sum of base-100 digits of k)
 *          applied to n.
 * https://oeis.org/A292218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (f^n)(n) where f(k)=k+digit_sum_base100(k)
 * Range:     u8 n <= 5, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* digit_sum_base100(k) = sum of digits of k in base 100 */

static inline seq_t sum_base100_digits(seq_t x)
{
    seq_t sum = 0;
    while (x > 0) {
        sum += x % (seq_t)100;
        x /= (seq_t)100;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1, but guard against 0 */

    seq_t val = (seq_t)n;          /* start with n */
    for (idx_t i = 0; i < n; ++i) {
        val += sum_base100_digits(val);
    }
    return val;
}

/*
 * A364099  Expansion of Sum_{k>0} k * x^k / (1 - x^(5*k-4)).
 * https://oeis.org/A364099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/5) * Sum_{d | (5*n-4), d == 1 (mod 5)} (d + 4)
 * Range:     u8 n <= 43, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        /* the sequence is defined for n >= 1; behaviour below offset doesn't matter */
        return 0;
    }

    /* m = 5*n - 4, the argument whose divisors we inspect */
    idx_t m = 5 * n - 4;

    seq_t sum = 0;

    /* iterate over all divisors d of m */
    for (idx_t d = 1; d * d <= m; ++d) {
        if (m % d == 0) {
            /* first divisor d */
            if (d % 5 == 1)
                sum += (seq_t)d + 4;

            /* complementary divisor e = m / d, avoid double counting the square root */
            idx_t e = m / d;
            if (e != d && e % 5 == 1)
                sum += (seq_t)e + 4;
        }
    }

    /* The sum is always divisible by 5, so we can safely divide */
    return sum / (seq_t)5;
}

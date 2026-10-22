/*
 * A364019  Expansion of Sum_{k>0} k * x^k / (1 + x^(5*k)).
 * https://oeis.org/A364019
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k|n, n/k≡1 (mod 5)} k · (−1)^((n/k−1)/5)
 * Range:     u8 n <= 230, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* use signed intermediate to handle the occasional subtraction */
    long long sum = 0;

    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d != 0)
            continue;

        /* first divisor k = d, m = n/d */
        idx_t k = d;
        idx_t m = n / k;
        if (m % 5 == 1) {
            idx_t t = (m - 1) / 5;
            if ((t & 1) == 0)
                sum += (long long)k;   /* even t → + sign */
            else
                sum -= (long long)k;   /* odd  t → – sign */
        }

        /* complementary divisor k = n/d, m = d */
        if (d != n / d) {
            k = n / d;
            m = d; /* because m = n/k */
            if (m % 5 == 1) {
                idx_t t = (m - 1) / 5;
                if ((t & 1) == 0)
                    sum += (long long)k;
                else
                    sum -= (long long)k;
            }
        }
    }

    return (seq_t)sum;
}

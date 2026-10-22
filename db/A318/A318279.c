/*
 * A318279  a(n) is the least k such that k^(tau(n)-1) >= n^tau(n).
 * https://oeis.org/A318279
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil(n^(1 + 1/(tau(n)-1)))
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 9862, u64 n <= 9862,
 *            u128 n <= 9862, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* tau(n) = number of positive divisors of n */

#include <math.h>

static idx_t divisor_count(idx_t n)
{
    idx_t count = 1;
    idx_t tmp = n;
    for (idx_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            idx_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            count *= (exp + 1);
        }
    }
    if (tmp > 1) {
        count *= 2;
    }
    return count;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;      /* offset starts at 2; below offset can be any value */

    /* divisor count */
    idx_t tau = divisor_count(n);    /* tau >= 2 for n >= 2 */

    /* compute the bound for binary search: k <= n^2  */
    idx_t high = n * n;
    idx_t low = 1;

    long double log_n = logl((long double)n);
    const long double eps = 1e-12L;

    while (low < high) {
        idx_t mid = (low + high) / 2;
        /* compare mid^(tau-1) >= n^tau  using logarithms */
        long double log_left  = (tau - 1) * logl((long double)mid);
        long double log_right = tau * log_n;

        if (log_left >= log_right - eps) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    return (seq_t)low;
}

/*
 * A344867  a(n) = n * Sum_{d|n} floor(sqrt(d)) / d.
 * https://oeis.org/A344867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * Sum_{d|n} floor(sqrt(d)) / d, where d runs over all
 *            positive divisors of n
 * Range:     u8 n <= 59, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: returns floor(sqrt(n)) using only integer arithmetic. */
static inline idx_t _isqrt(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    idx_t mid, best = 0;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        if (mid == 0) {
            best = 0;
            low = 1;
            continue;
        }
        if (mid <= n / mid) {  /* mid*mid <= n without overflow */
            best = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return best;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* offset is 1; return 0 for safety */

    seq_t sum = 0;
    idx_t i;

    for (i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            seq_t sqrt_d1 = (seq_t)_isqrt(d1);
            seq_t sqrt_d2 = (seq_t)_isqrt(d2);

            seq_t k1 = (seq_t)(n / d1);   /* k1 = n/d1 = d2 */
            sum += k1 * sqrt_d1;

            if (d1 != d2) {
                seq_t k2 = (seq_t)(n / d2);   /* k2 = n/d2 = d1 */
                sum += k2 * sqrt_d2;
            }
        }
    }
    return sum;
}

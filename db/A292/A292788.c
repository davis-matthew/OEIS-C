/*
 * A292788  For n > 1, a(n) = least positive k, not a power of n, such that
 *          the digital sum of k in base n equals the digital sum of k^3 in
 *          base n.
 * https://oeis.org/A292788
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = least positive k (not a power of n) such that
 *            digit_sum_base(k,n) == digit_sum_base(k^3,n)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static inline seq_t digit_sum_base(seq_t val, idx_t base)
{
    seq_t sum = 0;
    seq_t b = (seq_t)base;
    while (val > 0) {
        sum += val % b;
        val /= b;
    }
    return sum;
}

static inline bool is_power_of_n(seq_t val, idx_t base)
{
    if (val == 1) return true;   /* n^0 */
    seq_t b = (seq_t)base;
    if (val < b) return false;   /* cannot be a power */
    while (val % b == 0) {
        val /= b;
        if (val == 1) return true;
        if (val < b) return false;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;   /* undefined for n<=1, return 0 */
    seq_t bn = (seq_t)n;
    seq_t k = 2;            /* start from 2 (1 is a power of n) */
    while (1) {
        if (!is_power_of_n(k, n)) {
            seq_t ds1 = digit_sum_base(k, n);
            seq_t k3 = k * k * k;
            seq_t ds2 = digit_sum_base(k3, n);
            if (ds1 == ds2) return k;
        }
        ++k;
    }
}

/*
 * A245093  Triangle read by rows in which row n lists the first n terms of
 *          A000203.
 * https://oeis.org/A245093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, look
 * Formula:   a(n) = σ(n - T_{r-1}) where r = smallest integer with T_r ≥ n
 *            and T_m = m(m+1)/2. Here σ(k) is the sum of divisors of k
 * Range:     u8 n <= 5885, u16 n <= 7875, u32 n <= 7875, u64 n <= 7875,
 *            u128 n <= 7875, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* compute sum of divisors of k (k ≥ 1) */
static seq_t sigma_of(idx_t k)
{
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= k; ++d) {
        if (k % d == 0) {
            sum += (seq_t)d;
            idx_t other = k / d;
            if (other != d)
                sum += (seq_t)other;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Find r so that T_r ≥ n (T_r = r(r+1)/2) */
    idx_t r = 1;
    idx_t total = 1;            /* T_1 */
    while (total < n) {
        ++r;
        total += r;             /* total becomes T_r */
    }
    /* total = T_r, so T_{r-1} = total - r */
    idx_t k = n - (total - r);  /* 1 ≤ k ≤ r */
    return sigma_of(k);
}

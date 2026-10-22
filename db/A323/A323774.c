/*
 * A323774  Number of multiset partitions, whose parts are constant and all
 *          have the same sum, of integer partitions of n.
 * https://oeis.org/A323774
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} binomial(tau(d) + n/d - 1, n/d)
 * Range:     u8 n <= 35, u16 n <= 215, u32 n <= 1679, u64 n <= 9771,
 *            u128 n <= 9771, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where tau(k) is the number of positive divisors of k (A000005). */

#include <stdint.h>   /* for uint64_t */

/* Return the divisor count τ(k) for 0 < k <= 10000. */
static idx_t tau_uint64(uint64_t k)
{
    idx_t cnt = 1;
    uint64_t x = k;
    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            idx_t exp = 0;
            while (x % p == 0) {
                x /= p;
                ++exp;
            }
            cnt *= (exp + 1);
        }
    }
    if (x > 1) {            /* remaining prime factor */
        cnt *= 2;
    }
    return cnt;
}

/* Compute binomial(n, k) exactly as seq_t (k <= n). */
static seq_t binom_seq(idx_t n, idx_t k)
{
    if (k == 0 || k == n) return (seq_t)1;
    if (k > n / 2) k = n - k;                /* use smaller k */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* multiply first, then divide to keep intermediate integer */
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t total = 0;
    for (uint64_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            uint64_t d2 = n / d;                /* complementary divisor */

            /* term for divisor d */
            idx_t tau_d = tau_uint64(d);
            seq_t term = binom_seq((idx_t)(tau_d + d2 - 1), (idx_t)d2);
            total += term;

            if (d != d2) {
                /* term for divisor d2 */
                idx_t tau_d2 = tau_uint64(d2);
                seq_t term2 = binom_seq((idx_t)(tau_d2 + d - 1), (idx_t)d);
                total += term2;
            }
        }
    }
    return total;
}

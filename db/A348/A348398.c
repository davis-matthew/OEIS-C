/*
 * A348398  a(n) = Sum_{d|n} sigma_[n/d](d), where sigma_[k](n) is the sum of
 *          the k-th powers of the divisors of n.
 * https://oeis.org/A348398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} sigma_{n/d}(d)
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 32, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/factor_u64.h"

/* Integer square root: floor(sqrt(n)) */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (mid != 0 && mid > n / mid) {
            high = mid - 1;
        } else {
            idx_t nxt = mid + 1;
            if (nxt != 0 && nxt > n / nxt) {
                return mid;
            }
            low = nxt;
        }
    }
    return n; /* unreachable, keeps compiler happy */
}

/* Sum of k‑th powers of divisors of d */
static seq_t sigma_k_of_d(idx_t d, idx_t k)
{
    static uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64(d, pr, ex, &cnt);
    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p   = (seq_t)pr[i];
        seq_t num = big_pow(p, (seq_t)(k * (ex[i] + 1))) - 1;
        seq_t den = big_pow(p, (seq_t)k) - 1;
        seq_t term = num / den;
        res *= term;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n=0, offset starts at 1 */
    seq_t result = 0;
    idx_t sq = isqrt_u64(n);
    for (idx_t d = 1; d <= sq; ++d) {
        if (n % d == 0) {
            idx_t d1 = d;
            idx_t d2 = n / d;
            result += sigma_k_of_d(d1, d2);
            if (d1 != d2) {
                result += sigma_k_of_d(d2, d1);
            }
        }
    }
    return result;
}

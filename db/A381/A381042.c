/*
 * A381042  Alternating sum of floor(n^(1/k)), with k >= 2.
 * https://oeis.org/A381042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>=1} ( floor(n^(1/(2*k))) - floor(n^(1/(2*k+1))) )
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: a(0) = 0, a(1) = 0, a(2) = 0, a(3) = 0, a(4) = 1, …          */

#include <stdint.h>
#include <stdbool.h>

/* integer k‑th root: largest r such that r^k <= n (k >= 1, n >= 0)   */
static inline uint64_t kth_root_uint64(idx_t n, unsigned k)
{
    if (k == 1) return (uint64_t)n;

    uint64_t low = 1, high = (uint64_t)n;
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;

        /* Check whether mid^k <= n without overflow.            */
        uint64_t prod = 1;
        bool ok = true;
        for (unsigned i = 0; i < k; ++i) {
            if (prod > (uint64_t)n / mid) {
                ok = false;
                break;
            }
            prod *= mid;
        }

        if (ok)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;        /* a(0) = 0 */

    seq_t sum = (seq_t)0;

    /* For each pair of exponents (2k, 2k+1) compute their floors. */
    for (unsigned k = 1;; ++k) {
        unsigned exp_even = 2 * k;          /* 2k   */
        unsigned exp_odd  = 2 * k + 1;      /* 2k+1 */

        uint64_t r_even = kth_root_uint64(n, exp_even);
        uint64_t r_odd  = kth_root_uint64(n, exp_odd);

        /* Once both roots equal 1, all further differences are zero.   */
        if (r_even == 1 && r_odd == 1)
            break;

        sum += (seq_t)(r_even - r_odd);
    }

    return sum;
}

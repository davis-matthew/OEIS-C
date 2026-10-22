/*
 * A347398  Expansion of g.f. Sum_{k>=1} k^k * x^(k^k)/(1 - x^(k^k)).
 * https://oeis.org/A347398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k ≥ 1, k^k | n} k^k
 * Range:     u8 n <= 26, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t k = 1; ; ++k) {
        seq_t p = 1;
        for (idx_t i = 0; i < k; ++i) {
            p *= (seq_t)k;          /* compute k^k */
        }
        if (p > (seq_t)n) break;    /* no larger k can satisfy k^k ≤ n */
        if ((uint64_t)n % (uint64_t)p == 0) { /* n is uint64_t */
            sum += p;
        }
    }
    return sum;
}

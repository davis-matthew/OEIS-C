/*
 * A347143  Sum of 4th powers of divisors of n that are <= sqrt(n).
 * https://oeis.org/A347143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d | n, d ≤ sqrt(n)} d^4 (Sum of fourth powers of
 *            divisors ≤ √n)
 * Range:     u8 n <= 15, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast integer square root for unsigned 64-bit values */
static inline idx_t isqrt_u64(idx_t n) {
    idx_t r = 0;
    for (int i = 63; i >= 0; --i) {
        idx_t bit = 1ULL << i;
        if (r + bit <= n / (r + bit)) {
            r += bit;
        }
    }
    return r;
}

seq_t a(idx_t n) {
    idx_t limit = isqrt_u64(n);
    seq_t sum = 0;
    for (idx_t d = 1; d <= limit; ++d) {
        if (n % d == 0) {
            seq_t val = (seq_t)d;
            val = val * val; /* d^2 */
            val = val * val; /* d^4 */
            sum += val;
        }
    }
    return sum;
}

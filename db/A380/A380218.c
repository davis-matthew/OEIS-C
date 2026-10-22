/*
 * A380218  Number of integer partitions of n with product n+1.
 * https://oeis.org/A380218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer partitions of n with product n+1
 * Range:     u8 n <= 2878, u16 n <= 9846, u32 n <= 9846, u64 n <= 9846,
 *            u128 n <= 9846, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t dfs(uint64_t rem, uint64_t min_f, idx_t sum, idx_t limit) {
    seq_t total = 0;
    for (uint64_t f = min_f; f * f <= rem; ++f) {
        if (rem % f == 0) {
            idx_t new_sum = sum + (idx_t)f;
            if (new_sum > limit) continue;
            uint64_t rem2 = rem / f;
            if (rem2 == 1) {
                total += 1;
            } else {
                total += dfs(rem2, f, new_sum, limit);
            }
        }
    }
    if (rem >= min_f) {
        idx_t new_sum = sum + (idx_t)rem;
        if (new_sum <= limit) {
            total += 1;
        }
    }
    return total;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    idx_t prod  = n + 1;  /* product to be factored */
    idx_t limit = n;      /* sum of factors must be <= n */
    return dfs((uint64_t)prod, 2, 0, limit);
}

/*
 * A321441  Number of "hexagonal partitions" of n.
 * https://oeis.org/A321441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of hexagonal partitions of n (see OEIS entry
 *            A321441). The algorithm enumerates all triples (k, m, s)
 *            describing a partition: * k = smallest part, * m = largest part
 *            (k ≤ m), * s = split point between single‑multiplicity parts
 *            (k-1 ≤ s ≤ m-1). Parts ≤ s appear once, parts s+1 … m-1...
 * Range:     u8 n <= 383, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;          /* empty partition */

    /* prefix sums of 1..i (long long to avoid overflow) */
    long long *pre = (long long *)malloc((n + 1) * sizeof(long long));
    pre[0] = 0;
    for (idx_t i = 1; i <= n; ++i) pre[i] = pre[i - 1] + (long long)i;

    long long count = 0;                   /* final answer fits in seq_t */

    for (idx_t m = 1; m <= n; ++m) {
        /* iterate k downward; base_sum grows as k decreases,
           so stop when it exceeds n                  */
        for (idx_t k = m; k >= 1; --k) {
            /* base sum of once‑multiplicity parts: Σ_{i=k}^{m} i */
            long long base_sum = (long long)(k + m) * (m - k + 1) / 2;
            if (base_sum > (long long)n) break;

            /* total sum of all parts except the last one */
            long long total_others = pre[m - 1] - pre[k - 1];
            /* C = n - (2*total_others + m)          */
            long long C = (long long)n - (2 * total_others + (long long)m);

            /* iterate over possible split points s
               (k-1 ≤ s ≤ m-1).  sum_k_s = Σ_{i=k}^{s} i, initially 0
               and increased by i when s ≥ k. */
            long long sum_k_s = 0;
            for (idx_t s = k - 1; s <= m - 1; ++s) {
                if (s >= k) sum_k_s += (long long)s;

                long long RHS = C + sum_k_s;   /* RHS = (t-1)m  */
                if (RHS >= 0) {
                    if (RHS % (long long)m == 0) {
                        /* t = 1 + RHS / m  ≥ 1 */
                        ++count;
                    }
                }
            }
        }
    }

    free(pre);
    return (seq_t)count;
}

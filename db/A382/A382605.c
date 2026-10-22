/*
 * A382605  Number of distinct solutions to the problem of folding in half a
 *          chain of linked rods of length 1, ..., n.
 * https://oeis.org/A382605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 63, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A382605: Number of distinct solutions to folding a chain of linked rods
 * of lengths 1..n in half.
 *
 * A solution is a pair (k, r) with 0 <= k < n, 0 < r < n-k such that the
 * first k rods together with the first r rods of the reversed suffix
 * (n, n-1, ..., k+1) sum to the half of the total length. Each unordered
 * bipartition of the rods with equal lengths corresponds to exactly one
 * such (k, r) with r > 0, providing a canonical representation. This
 * function counts those solutions.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    if (n < 2) return 0;

    seq_t total = (seq_t)n * ((seq_t)n + 1) / 2;      /* total length */
    seq_t ans = 0;

    /* 2*n + 1 */
    seq_t a_val = ((seq_t)n << 1) + 1;

    for (idx_t k = 0; k < n; ++k) {
        seq_t k_seq = (seq_t)k;
        seq_t U = k_seq * (k_seq + 1) / 2;        /* sum of first k rods */

        seq_t twoU = U << 1;
        if (twoU > total) continue;              /* X would be negative */

        seq_t X = total - twoU;                  /* non‑negative */

        seq_t fourX = X << 2;
        seq_t a2 = a_val * a_val;
        if (fourX > a2) continue;                /* D would be negative */

        seq_t D = a2 - fourX;                    /* discriminant */

        seq_t sqrtD = floor_sqrt(D);
        if (sqrtD * sqrtD != D) continue;        /* not a perfect square */

        seq_t diff = a_val - sqrtD;
        if (diff & 1) continue;                  /* r not integer */

        seq_t r = diff >> 1;                     /* number of suffix rods */

        if (r == 0) continue;                    /* canonicalise: r > 0 */

        /* ensure the prefix does not include the whole chain */
        seq_t limit = (seq_t)n - k;
        if (r >= limit) continue;                /* r must be < n - k */

        /* one valid solution found */
        ++ans;
    }

    return ans;
}

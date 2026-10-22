/*
 * A363326  Total number of parts coprime to n in the partitions of n into 8
 *          parts.
 * https://oeis.org/A363326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Total number of parts coprime to n in the partitions of n
 *            into 8 parts
 * Range:     u8 n <= 18, u16 n <= 46, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes it by dynamic programming on the number of partitions
   into a fixed number of parts and uses the recurrence
   M_k(n,d) = Σ_{j=1..k} p_{j-1}(n - (k+1-j)d),
   where p_k(m) is the number of partitions of m into exactly k parts. */
#include <stdlib.h>
#include <stdint.h>

/* Forward declaration of seq_t and idx_t types – provided by the build system */
typedef uint64_t idx_t;   /* placeholder, real type may differ */

/* Basic gcd for idx_t */
static inline idx_t gcd_idx(idx_t a, idx_t b) {
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset 1, but safe handling */
    /* Allocate DP table: p[k][m] for k=0..7, m=0..n */
    size_t sz = (size_t)(n + 1);
    seq_t *p = (seq_t *)malloc(8 * sz * sizeof(seq_t));
    if (!p) return 0;              /* allocation failure */

    /* Initialize p[0][0] = 1 */
    for (size_t m = 0; m < sz; ++m) {
        p[m] = (m == 0) ? 1 : 0;
    }

    /* Compute p[k][m] for k=1..7 */
    for (int k = 1; k <= 7; ++k) {
        seq_t *prow = p + k * sz;
        seq_t *prow_prev = p + (k - 1) * sz;
        for (size_t m = 0; m < sz; ++m) {
            seq_t val = 0;
            if (m >= (size_t)k) val += prow[m - k];
            if (m >= 1) val += prow_prev[m - 1];
            prow[m] = val;
        }
    }

    seq_t total = 0;
    for (idx_t d = 1; d <= n; ++d) {
        if (gcd_idx(d, n) != 1) continue;  /* only coprime parts matter */

        seq_t subtotal = 0;
        for (int j = 1; j <= 8; ++j) {
            idx_t coeff = (idx_t)(9 - j);            /* coeff = k+1-j for k=8 */
            idx_t mult = coeff * d;
            if (mult > n) continue;
            idx_t idx = n - mult;
            subtotal += p[(j - 1) * sz + idx];
        }
        total += subtotal;
    }

    free(p);
    return total;
}

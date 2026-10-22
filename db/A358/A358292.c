/*
 * A358292  Array read by antidiagonals: T(n,k) = n^3*k^3*(n+k)^2, n>=0, k>=0.
 * https://oeis.org/A358292
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the n‑th element of the array read by antidiagonals of
 *            T(n,k) = n³·k³·(n+k)², with n,k ≥ 0. The ordering is (n,k) =
 *            (0,0), (0,1),(1,0), (0,2),(1,1),(2,0), …
 * Range:     u8 n <= 10, u16 n <= 30, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <math.h>

seq_t a(idx_t n) {
    /* Find the antidiagonal (s) that contains index n.
       The total number of terms up to antidiagonal s-1 is
       count = s*(s+1)/2.  We need the largest s such that
       count <= n.  Use integer arithmetic to avoid
       precision issues. */
    idx_t s = (idx_t)((sqrtl((long double)8 * n + 1) - 1) / 2);

    /* Correct possible rounding errors */
    while ((s + 1) * (s + 2) / 2 <= n) ++s;
    while (s * (s + 1) / 2 > n) --s;

    /* offset within the antidiagonal */
    idx_t offset = n - s * (s + 1) / 2;
    /* In the order (k = 0 .. s), n_index = s - k */
    idx_t k = offset;         /* 0 <= k <= s */
    idx_t n_idx = s - k;      /* 0 <= n_idx <= s */

    seq_t n_seq = (seq_t)n_idx;
    seq_t k_seq = (seq_t)k;
    seq_t sum = n_seq + k_seq;

    /* Compute n³ · k³ · (n+k)² */
    seq_t term = n_seq * n_seq * n_seq;      /* n³ */
    term *= k_seq * k_seq * k_seq;           /* · k³ */
    term *= sum * sum;                       /* · (n+k)² */

    return term;
}

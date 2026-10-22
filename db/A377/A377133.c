/*
 * A377133  Triangle read by rows: T(n,k) is the maximum volume of an
 *          integer-sided box that can be made from a piece of paper of size n
 *          X k by cutting away identical squares at each corner and folding
 *          up the sides, n >= 3, 3 <= k <= n.
 * https://oeis.org/A377133
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) enumerates the triangle T(n,k) read by rows, * starting
 *            with n = 3, k = 3. For n >= 3, k runs from 3 to n * and T(n,k) =
 *            (n-2*x)*(k-2*x)*x with * x = round((n + k - sqrt(n^2 + k^2 -
 *            n*k)) / 6). The result is * the maximum integer‐volume box that
 *            can be made from an n × k * sheet of paper. Th...
 * Range:     u8 n <= 105, u16 n <= 4466, u32 n <= 9872, u64 n <= 9872,
 *            u128 n <= 9872, big any n
 * Helpers:   volume
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/volume.h"

/* Integer square root for uint64_t: floor(sqrt(v)) */
static inline idx_t int_sqrt_u64(idx_t v)
{
    idx_t low = 0, high = 0xFFFFFFFFull;          /* sqrt(2^64-1) <= 2^32-1 */
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 prod = (unsigned __int128)mid * (unsigned __int128)mid;
        if (prod <= v) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 3) return 0;                /* undefined for offset below 3 */

    /* Convert to 1‑based position within the triangle. */
    idx_t pos = n - 2;                  /* a(3) -> pos = 1 */

    /* Find the row N such that cumulative count up to row N-1
     * is < pos <= cumulative up to row N.  The cumulative count
     * is (N-2)(N-1)/2 for N >= 3. */
    idx_t N = 3;
    while ((N - 2) * (N - 1) / 2 < pos) {
        ++N;
    }

    /* Position of pos within row N (1‑based). */
    idx_t prev = (N - 3) * (N - 2) / 2;
    idx_t offset = pos - prev;             /* 1, 2, …, N-2 */
    idx_t K = offset + 2;                  /* k starts at 3 */

    /* Compute an approximate x.  The exact value is
     * round((n+k - sqrt(n^2 + k^2 - n*k)) / 6).  Since we use
     * integer arithmetic we evaluate the floor of the square
     * root and then test neighboring integers to ensure the
     * maximum. */
    idx_t val = N * N + K * K - N * K;     /* fits in idx_t for n up to ~10⁴ */
    idx_t s = int_sqrt_u64(val);
    idx_t x0 = (N + K >= s) ? (N + K - s) / 6 : 0;

    /* The feasible range for x is 1 <= x <= min((N-1)/2, (K-1)/2). */
    idx_t max_x = (N - 1) / 2;
    idx_t k_lim = (K - 1) / 2;
    if (k_lim < max_x) max_x = k_lim;

    /* Search in a small neighbourhood of the estimate. */
    seq_t best = 0;
    for (int d = -2; d <= 2; ++d) {
        idx_t cand = x0 + d;
        if (cand < 1 || cand > max_x) continue;
        seq_t v = volume(N, K, cand);
        if (v > best) best = v;
    }

    return best;
}

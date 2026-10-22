/*
 * A385214  Number of subsets of {1..n} without all equal lengths of maximal
 *          runs of consecutive elements increasing by 1.
 * https://oeis.org/A385214
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 2^n - 1 - Σ_{L=1..n} Σ_{k=1..⌊(n+1)/(L+1)⌋} C(r + k, k),
 *            where r = n - k*L - (k-1). Counts subsets of {1..n} whose
 *            maximal runs all have equal length; subtract from all non‑empty
 *            subsets
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t good_sum = (seq_t)0;

    for (idx_t L = 1; L <= n; ++L) {
        /* maximum number of runs for this L */
        idx_t kmax = (n + 1) / (L + 1);
        for (idx_t k = 1; k <= kmax; ++k) {
            idx_t needed = k * L + (k - 1);
            if (n < needed)   /* safety, should not happen due to kmax */
                continue;
            idx_t r = n - needed;          /* remaining zeros after mandatory gaps */
            idx_t N = r + k;                /* total objects for the stars‑and‑bars count */

            /* Compute C(N, k) multiplicatively, using the smaller of k and N-k */
            idx_t kk = k;
            if (kk > N - kk)
                kk = N - kk;

            seq_t c = (seq_t)1;
            for (idx_t i = 1; i <= kk; ++i) {
                c = c * (seq_t)(N - kk + i);
                c = c / (seq_t)i;
            }

            good_sum += c;
        }
    }

    seq_t total   = pow2(n);            /* 2^n */
    seq_t answer  = total - (seq_t)1 - good_sum; /* exclude empty set */
    return answer;
}

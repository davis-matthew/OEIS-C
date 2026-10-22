/*
 * A390973  a(n) = (1/(2*n+1)) * Sum_{k=0..n} k^4 * (2*k+1) *
 *          binomial(3*n-k,n-k).
 * https://oeis.org/A390973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/(2*n+1) * Σ_{k=0..n} k^4 * (2k+1) * binomial(3*n-k,
 *            n-k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 46,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* If seq_t is small (≤64‑bit) use 128‑bit arithmetic for safety. */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 bin = 1;
        idx_t N = n;

        /* bin = C(3N, N) */
        for (idx_t i = 1; i <= N; ++i) {
            bin *= (unsigned __int128)(3 * N - (i - 1));
            bin /= i;
        }

        unsigned __int128 sum = 0;
        for (idx_t k = 0; k <= N; ++k) {
            unsigned __int128 kk   = k;
            unsigned __int128 kk4  = kk * kk * kk * kk;          /* k^4 */
            unsigned __int128 fact = (unsigned __int128)(2 * k + 1);
            unsigned __int128 term = kk4 * fact * bin;
            sum += term;

            if (k < N) {
                bin = bin * (unsigned __int128)(N - k) /
                      (unsigned __int128)(3 * N - k);
            }
        }
        sum /= (unsigned __int128)(2 * N + 1);
        return (seq_t)sum;
    }

    /* For arbitrary‑precision seq_t use the provided binomial routine. */
    seq_t bin   = binomial((3 * n), n);
    seq_t sum   = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t kk   = (seq_t)k;
        seq_t kk4  = kk * kk * kk * kk;                 /* k^4 */
        seq_t fact = (seq_t)(2 * k + 1);
        seq_t term = kk4 * fact * bin;
        sum += term;

        if (k < n)
            bin = bin * (seq_t)(n - k) / (seq_t)(3 * n - k);
    }
    return sum / (seq_t)(2 * n + 1);
}

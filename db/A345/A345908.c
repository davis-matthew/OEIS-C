/*
 * A345908  Traces of the matrices (A345197) counting integer compositions by
 *          length and alternating sum.
 * https://oeis.org/A345908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n whose length k satisfies 2k =
 *            n + S, where S is the alternating sum of the composition. This
 *            is equivalent to counting compositions for each k such that the
 *            sum of parts at odd positions equals k and the sum of parts at
 *            even positions equals n - k
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 36, u64 n <= 68, u128 n <= 133,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Binomial coefficient: binom(a,b) is provided by the build */
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t total = 0;
    for (idx_t k = 1; k <= n; ++k) {
        /* number of odd and even positions */
        idx_t odd = (k + 1) / 2;   /* ceil(k/2) */
        idx_t even = k / 2;        /* floor(k/2) */

        /* ways to distribute the odd-part sum = k over odd positions */
        seq_t ways_odd = binom(k - 1, odd - 1);

        seq_t ways_even;
        if (even == 0) {
            /* no even positions – possible only when the even sum is 0 */
            ways_even = (n == k) ? (seq_t)1 : (seq_t)0;
        } else {
            idx_t even_sum = n - k;            /* must equal sum of even parts */
            if (even_sum < even) {
                ways_even = 0;
            } else {
                ways_even = binom(even_sum - 1, even - 1);
            }
        }

        if (ways_odd && ways_even) {
            total += ways_odd * ways_even;
        }
    }
    return total;
}

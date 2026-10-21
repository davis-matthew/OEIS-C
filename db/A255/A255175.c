/*
 * A255175  Expansion of phi(-x) / (1 - x)^2 in powers of x where phi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A255175
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{i=0}^n (-1)^{floor(sqrt(i))}
 * Range:     i8 n <= 63, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implemented using block‑wise accumulation to avoid a per‑term sqrt. */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert the index to seq_t for arithmetic later */
    seq_t sn = (seq_t)n;
    /* Highest integer k with k^2 <= n */
    idx_t m = (idx_t)floor_sqrt(sn);
    seq_t sum = 0;
    seq_t nn_plus1 = sn + 1;   /* n + 1, used for block limits */

    for (idx_t k = 0; k <= m; ++k) {
        /* Compute k^2 and (k+1)^2 safely in seq_t */
        seq_t ksq   = (seq_t)k     * (seq_t)k;
        seq_t k1sq  = (seq_t)(k+1) * (seq_t)(k+1);

        /* Count of indices i in this block: i = k^2 .. min((k+1)^2-1, n) */
        seq_t count = (nn_plus1 <= k1sq) ? (nn_plus1 - ksq) : (k1sq - ksq);

        /* Add or subtract the block based on parity of k */
        if ((k & 1u) == 0) {
            sum += count;   /* even floor sqrt → +1 */
        } else {
            sum -= count;   /* odd floor sqrt  → -1 */
        }
    }
    return sum;
}

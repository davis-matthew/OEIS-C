/*
 * A277337  Number of pairs of functions (f,g) from a set of n elements into
 *          itself that are generalized reflexive inverses of each other.
 * https://oeis.org/A277337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} ( n!/(n‑k)! * C(n,k) * k^(2*(n‑k)) )
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   pow_seq_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq_idx.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t falling = 1;   /* n!/(n‑k)! for current k */
    seq_t binom   = 1;   /* C(n,k)   for current k */

    for (idx_t k = 0; k <= n; ++k) {
        idx_t exp = 2 * (n - k);                /* exponent for k^(2*(n‑k)) */
        seq_t power = pow_seq_idx((seq_t)k, exp);

        seq_t term = falling;
        term *= binom;
        term *= power;
        sum += term;

        if (k < n) {
            falling *= (seq_t)(n - k);          /* update for next k+1 */
            binom   *= (seq_t)(n - k);
            binom   /= (seq_t)(k + 1);
        }
    }
    return sum;
}

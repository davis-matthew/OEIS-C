/*
 * A350646  Maximum number of inverses of an element in the full symmetric
 *          semigroup T_n.
 * https://oeis.org/A350646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{1≤k≤n} ( ⌊n/k⌋^(k−(n mod k)) · (⌊n/k⌋+1)^(n mod k) ·
 *            k^(n−k) )
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 24, u128 n <= 41,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 1; f is defined for all n≥0. */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t best = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t a_part = n / k;          /* floor(n/k)  */
        idx_t r      = n % k;          /* number of (a+1) parts   */
        seq_t term   = 1;

        /* product of the mi's: a_part^(k−r) * (a_part+1)^r */
        if (k > r) {
            term *= big_pow((seq_t)a_part, (seq_t)(k - r));
        }
        if (r > 0) {
            term *= big_pow((seq_t)(a_part + 1), (seq_t)r);
        }

        /* multiply by k^(n-k) */
        if (n > k) {
            term *= big_pow((seq_t)k, (seq_t)(n - k));
        }

        if (term > best) best = term;
    }
    return best;
}

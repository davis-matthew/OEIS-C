/*
 * A378426  Expansion of (1/x) * Series_Reversion( x / (1 + x + x^2 * (1 +
 *          x)^2) ).
 * https://oeis.org/A378426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Sum_{k=0..⌊n/2⌋} binomial(n+1,k) *
 *            binomial(n+k+1, n-2k)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 35, u128 n <= 66,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Compute a(n) for the OEIS sequence A378426 */
seq_t a(idx_t n)
{
    /* Special case: a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    idx_t kmax = n / 2;

    /* Use 128‑bit accumulation if seq_t is small enough (e.g., uint64_t) */
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 sum = 0;
        for (idx_t k = 0; k <= kmax; ++k) {
            seq_t c1 = binomial(n + 1, k);
            seq_t c2 = binomial(n + k + 1, n - 2 * k);
            sum += (unsigned __int128)c1 * (unsigned __int128)c2;
        }
        return (seq_t)(sum / (unsigned __int128)(n + 1));
    }

    /* Generic big‑int branch */
    seq_t sum = (seq_t)0;
    for (idx_t k = 0; k <= kmax; ++k) {
        seq_t c1 = binomial(n + 1, k);
        seq_t c2 = binomial(n + k + 1, n - 2 * k);
        sum += c1 * c2;              /* relies on operator+ and operator* */
    }
    seq_t denom = (seq_t)(n + 1);
    return sum / denom;               /* relies on operator/ */
}

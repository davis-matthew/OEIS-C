/*
 * A268597  Smallest x such that x-1 mod phi(x) = n, or 0 if no such x exists.
 * https://oeis.org/A268597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest x ≥ n+1 such that (x-1) mod φ(x) == n
 * Range:     u8 n <= 24, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   phi_clean
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi_clean.h"

seq_t a(idx_t n)
{
    seq_t target = (seq_t)n;         /* n as seq_t for comparison */
    seq_t start  = target + (seq_t)1;/* candidates must be at least n+1 */

    for (seq_t x = start; ; ++x)
    {
        seq_t phi_val = phi_clean(x);
        if ((x - (seq_t)1) % phi_val == target)
            return x;                 /* found the smallest x */
    }

    return (seq_t)0; /* unreachable: conjecture says a(n) > 0 for all n */
}

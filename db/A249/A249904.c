/*
 * A249904  a(n) = mu(n) + sigma(n) - n.
 * https://oeis.org/A249904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = μ(n) + σ(n) - n
 * Range:     u8 n <= 143, u16 n <= 9719, u32 n <= 9719, u64 n <= 9719,
 *            u128 n <= 9719, big any n
 * Helpers:   mobius, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mobius.h"
#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    seq_t mu     = mobius(n);
    seq_t sigma  = sigma_of(n);
    seq_t nn     = (seq_t)n;
    return mu + sigma - nn;
}

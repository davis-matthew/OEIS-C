/*
 * A348930  a(n) = A038502(sigma(n)), where A038502 is fully multiplicative
 *          with a(3) = 1, and a(p) = p for any other prime p.
 * https://oeis.org/A348930
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sigma(n) with all factors of 3 removed (i.e., a(n) =
 *            sigma(n) / 3^v_3(sigma(n)))
 * Range:     u8 n <= 107, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    seq_t val = sigma_of(n);     /* compute σ(n) */
    while ((val % (seq_t)3) == 0) /* remove all factors of 3 */
        val /= 3;
    return val;
}

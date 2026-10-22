/*
 * A332054  Tower of 9's modulo n.
 * https://oeis.org/A332054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9^a(φ(n)) mod n
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   mod_pow, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;
    seq_t phi_n = phi(n);           /* φ(n) */
    seq_t exp = a((idx_t)phi_n);     /* a(φ(n)) */
    return mod_pow((seq_t)9, (uint64_t)exp, (seq_t)n);
}

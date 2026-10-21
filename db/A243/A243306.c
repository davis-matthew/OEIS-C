/*
 * A243306  2^phi(n) - phi(n).
 * https://oeis.org/A243306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^phi(n) - phi(n) (OEIS A243306)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 36, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   phi_of_idx, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"
#include "../utils/phi_of_idx.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Index 0 is below the OEIS offset; behaviour undefined. */
        return (seq_t)0;
    }
    seq_t phi_val = phi_of_idx(n);          /* φ(n)  */
    idx_t exp = (idx_t)phi_val;             /* exponent fits in idx_t for n ≤ 10000 */
    seq_t pow_val = seq_pow((seq_t)2, exp); /* 2^φ(n) */
    return pow_val - phi_val;               /* a(n) = 2^φ(n) - φ(n) */
}

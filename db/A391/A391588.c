/*
 * A391588  a(n) = A003817(n) XOR (A003817(n)*n).
 * https://oeis.org/A391588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ((n-1) * 2^(⌊log₂(n)⌋ + 1)) + (n-1)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 75 terms through n = 9812 (2026-11-01)
 * Helpers:   ilog2, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   = (n-1) * (2^(⌊log₂(n)⌋ + 1) + 1) */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/ilog2.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* OEIS offset is 1; for n == 0 just return 0 (unused). */
    if (n == 0)
        return 0;

    /* L = floor(log2(n)) + 1 */
    idx_t L = ilog2(n) + 1;

    /* Compute 2^L as seq_t */
    seq_t two_pow = pow2(L);

    /* (n-1) * (2^L + 1) */
    return ((seq_t)(n - 1)) * (two_pow + 1);
}

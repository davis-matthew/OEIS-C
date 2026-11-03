/*
 * A289612  a(n) is the number of permutations of length n that avoid the
 *          pattern 321 and the mesh pattern (12, 381) or the same sequence
 *          for the mesh pattern (12, 471).
 * https://oeis.org/A289612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Catalan(n+1) - 2^(n-1), a(0)=1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 27 (2026-11-01)
 * Helpers:   catalan, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/catalan.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t cat = catalan(n + 1);
    seq_t pw  = pow2(n - 1);
    return cat - pw;
}

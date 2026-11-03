/*
 * A349648  Expansion of g.f.: Catalan(x)/Catalan(-x).
 * https://oeis.org/A349648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n % 2 == 0) ? Catalan(n) : 2^n * Catalan((n-1)/2) where
 *            Catalan(k) is the k-th Catalan number
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Validated: partial, pass, 28 terms through n = 27 (2026-11-01)
 * Helpers:   catalan, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/catalan.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if ((n & 1) == 0) {            /* even index */
        return catalan(n);
    } else {                       /* odd index */
        return pow2(n) * catalan((n - 1) / 2);
    }
}

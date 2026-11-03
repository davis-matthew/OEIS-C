/*
 * A331799  Normalized volume of the Caracol flow polytope. Also equal to the
 *          number of "unified diagrams" of the Caracol graph (see Section 4.3
 *          and Section 5 in Benedetti et al. reference).
 * https://oeis.org/A331799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000108(n-1) * A000272(n+1) Equivalent: a(n) =
 *            catalan(n-1) * (n+1)^(n-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 17 (2026-11-01)
 * Helpers:   catalan, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/catalan.h"
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* offset starts at 1, guard against underflow */

    seq_t cat = catalan(n - 1);          /* Catalan number C_{n-1} */
    seq_t pow = ipow((seq_t)(n + 1), (seq_t)(n - 1));  /* (n+1)^(n-1) */

    return cat * pow;
}

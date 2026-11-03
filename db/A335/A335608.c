/*
 * A335608  Number of sets (in the Hausdorff metric geometry) at each location
 *          between two sets defined by a complete bipartite graph K(3,n)
 *          (with n at least 2) missing one edge.
 * https://oeis.org/A335608
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*7^(n-1) - 5*3^(n-1) + 2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 22 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* below offset: undefined behaviour OK */
    seq_t exp = (seq_t)(n - 1);   /* exponent as seq_t */

    seq_t p7 = ipow((seq_t)7, exp); /* 7^(n-1) */
    seq_t p3 = ipow((seq_t)3, exp); /* 3^(n-1) */

    seq_t res = p7 * 3;          /* 3 * 7^(n-1) */
    res -= p3 * 5;               /* - 5 * 3^(n-1) */
    res += 2;                    /* + 2 */

    return res;
}

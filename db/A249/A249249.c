/*
 * A249249  Total number of occurrences of the consecutive step pattern given
 *          by the binary expansion of n (where 1=up and 0=down) in all
 *          permutations of [n].
 * https://oeis.org/A249249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>0} k*A242783(n,k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   linear_extensions
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function below implements the definition by counting
 * the total number of occurrences of the binary pattern of n
 * in all permutations of [n].
 */

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/linear_extensions.h"

/* The build supplies the integer types seq_t and idx_t. */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* compute binary length of n (number of bits) */
    idx_t r = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        ++r;
        tmp >>= 1;
    }
    if (r >= n) return 0;                    /* no possible window */

    /* number of linear extensions for the pattern of n */
    uint64_t Lp = linear_extensions(n, r);

    /* compute n! / (r+1)!  as product from (r+2) to n */
    seq_t prod = 1;
    for (idx_t i = r + 2; i <= n; ++i) {
        prod *= (seq_t)i;
    }

    /* final result: (n - r) * Lp * prod */
    seq_t result = prod * (seq_t)Lp;
    result *= (seq_t)(n - r);
    return result;
}

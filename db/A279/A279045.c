/*
 * A279045  Number of pairs of vertices that share no common neighbor summed
 *          over all simple labeled graphs on n nodes.
 * https://oeis.org/A279045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^binomial(n,2) * binomial(n,2) * (3/4)^(n-2)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   binomial, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           = binomial(n,2) * 2^(binomial(n,2)-2*(n-2)) * 3^(n-2) */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* binomial coefficient C = n*(n-1)/2 */
    idx_t C = (n * (n - 1)) / 2;
    /* exponent for 2: C - 2*(n-2) */
    idx_t e2 = C - 2 * (n - 2);

    /* start with C as seq_t */
    seq_t result = binomial(n, 2);

    /* multiply by 2^e2 */
    result = result * pow2(e2);

    /* multiply by 3^(n-2) */
    result = result * pow3(n - 2);

    return result;
}

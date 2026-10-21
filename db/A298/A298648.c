/*
 * A298648  Number of smallest coverings of the n-dipyramidal graph by maximal
 *          cliques.
 * https://oeis.org/A298648
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2*k) = 2^(k+1) – 4, a(2*k-1) = (2*k-1)*(2^k – 2), a(3) = 1
 * Range:     u8 n <= 8, u16 n <= 22, u32 n <= 52, u64 n <= 114, u128 n <= 240,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;               /* indices below the offset are irrelevant */

    if (n == 3)
        return 1;               /* special value */

    if ((n & 1) == 0) {         /* even n = 2*k */
        idx_t k = n >> 1;
        seq_t v = pow2(k + 1);  /* 2^(k+1) */
        return v - 4;
    } else {                    /* odd n = 2*k-1, k > 2 */
        idx_t k = (n + 1) >> 1;
        seq_t v = pow2(k);      /* 2^k */
        v -= 2;
        return (seq_t)n * v;
    }
}

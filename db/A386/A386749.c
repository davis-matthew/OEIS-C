/*
 * A386749  a(n) = n*sigma_4(n).
 * https://oeis.org/A386749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n * sigma_4(n)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 83, u64 n <= 7031,
 *            u128 n <= 9000, big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/sigma_k.h"

/* Return the n-th term of OEIS sequence A386749.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* a(0) = 0 */

    seq_t sigma = sigma_k(n, 4);   /* sigma_4(n) */
    return (seq_t)n * sigma;       /* a(n) = n * sigma_4(n) */
}

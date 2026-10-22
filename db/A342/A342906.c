/*
 * A342906  a(n) = 2^(2*n-2) - Catalan(n).
 * https://oeis.org/A342906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2*n-2) - Catalan(n)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   catalan, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 1.  For n == 0 return 0 */
    if (n == 0)
        return 0;

    /* 2^(2*n-2) */
    seq_t power = pow2((idx_t)((seq_t)n * 2 - 2));

    /* Catalan(n) */
    seq_t cat = catalan(n);

    /* a(n) = 2^(2*n-2) - Catalan(n) */
    return power - cat;
}

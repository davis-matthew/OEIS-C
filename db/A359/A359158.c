/*
 * A359158  a(n) = 1 if the odd part of n is squarefree and the number of
 *          prime factors of n (with multiplicity) is odd, otherwise 0.
 * https://oeis.org/A359158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the odd part of n is squarefree and the total number
 *            of prime factors of n (with multiplicity) is odd; otherwise 0
 * Range:     u8 n <= 9649, u16 n <= 9649, u32 n <= 9649, u64 n <= 9649,
 *            u128 n <= 9649, big any n
 * Helpers:   bigomega, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/bigomega.h"
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* index 0 is outside the OEIS offset, safe guard */

    /* compute the odd part of n */
    idx_t odd_part = n;
    while ((odd_part & (idx_t)1) == 0)
        odd_part >>= 1;

    if (!is_squarefree((seq_t)odd_part))
        return 0;

    seq_t omega = bigomega(n);     /* total number of prime factors (with multiplicity) */
    return (omega & 1) ? 1 : 0;    /* 1 if odd, else 0 */
}

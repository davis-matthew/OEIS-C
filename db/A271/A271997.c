/*
 * A271997  The icosagen sequence : a(n) = A018227(n)-5, for n >= 2.
 * https://oeis.org/A271997
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*((n+3)^2 + 2)/6 + ((n%2==0)?(n+2)/2:0) - 6
 * Range:     u8 n <= 9, u16 n <= 71, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: a(n) is undefined for n < 2; behaviour there does not matter. */

#include <stdint.h> /* for uintptr_t types used by the build system */

seq_t a(idx_t n)
{
    /* compute with seq_t arithmetic only (no overflow concerns for the build types) */
    seq_t nn = (seq_t)n;                 /* cast index to seq_t            */
    seq_t s  = nn + (seq_t)3;            /* n + 3                          */
    seq_t s2 = s * s;                    /* (n + 3)^2                      */
    seq_t term1 = nn * (s2 + (seq_t)2) / (seq_t)6; /* n*((n+3)^2+2)/6        */

    /* extra term: (n+2)/2 when n is even, 0 otherwise */
    seq_t term2 = ((n & (idx_t)1) == 0) ? ((nn + (seq_t)2) / (seq_t)2)
                                         : (seq_t)0;

    return term1 + term2 - (seq_t)6;
}

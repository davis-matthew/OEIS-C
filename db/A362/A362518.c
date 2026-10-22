/*
 * A362518  Number of vertex cuts in the n-helm graph.
 * https://oeis.org/A362518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2^n - 1)*(1 + 2^(n + 1) - n) - 3^n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses only the provided pow2 and pow3 utilities. */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Ensure n >= 1. Behavior for n < 1 is undefined. */
    seq_t sn = (seq_t)n;                /* cast index to seq_t for arithmetic */

    seq_t p2n   = pow2(n);              /* 2^n  */
    seq_t p2n1  = pow2(n + 1);          /* 2^(n+1) */
    seq_t p3n   = pow3(n);              /* 3^n  */

    seq_t term1 = p2n - 1;              /* 2^n - 1 */
    seq_t term2 = 1 + p2n1 - sn;        /* 1 + 2^(n+1) - n */

    return (term1 * term2) - p3n;       /* (2^n-1)*(1+2^(n+1)-n) - 3^n */
}

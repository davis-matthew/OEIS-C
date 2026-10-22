/*
 * A332152  a(n) = 5*(10^(2*n+1)-1)/9 - 3*10^n.
 * https://oeis.org/A332152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 5*(10^(2*n+1)-1)/9 - 3*10^n, with a(0)=2
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A332152 exactly, using arbitrary‑precision arithmetic
   when available.  The computation is performed in a way that avoids
   intermediary overflow for the uint64_t build: we perform the division
   by 9 before the multiplication by 5.  The only used helper is
   pow10(idx_t) from the utils directory, which returns 10^e as a seq_t. */

#include <stdint.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* compute 5*(10^(2*n+1)-1)/9  */
    seq_t term = pow10(2*n + 1);   /* 10^(2*n+1)  */
    term -= 1;                     /* 10^(2*n+1)-1  */
    term /= 9;                     /* divide first to keep size small  */
    term *= 5;                     /* multiply by 5  */

    /* compute 3*10^n  */
    seq_t t = pow10(n);            /* 10^n  */
    t *= 3;                        /* 3*10^n  */

    return term - t;
}

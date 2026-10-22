/*
 * A306651  a(n) = Product_{k=1..n} BarnesG(3*k).
 * https://oeis.org/A306651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..n} BarnesG(3k) (BarnesG(1)=1,
 *            BarnesG(z+1)=Γ(z)·BarnesG(z))
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The Barnes G‑function for integers is
 * G(m+1) = (m-1)! · G(m), with G(1)=1.  We compute G at all integers up
 * to 3·n and multiply those where the index is a multiple of 3.  The
 * multiplication is done by the big‑integer helper seq_mul, which is
 * available in the build environment.  For n < 1 the undefined part below
 * the offset yields 0. */
       
#include <stdint.h>
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined below the offset */

    seq_t result = 1;                /* product of the requested Barnes G values */
    seq_t G      = 1;                /* current G(i)   (G(1) = 1) */
    seq_t fact   = 1;                /* (i-1)! for current i */

    idx_t limit = 3 * n;             /* highest index of G to compute */

    /* Loop over i = 1 .. limit-1 to compute G(i+1) */
    for (idx_t i = 1; i < limit; ++i) {
        /* G(i+1) = fact * G(i), where fact = (i-1)! */
        G = seq_mul(G, fact);

        /* If i+1 is a multiple of 3, include it in the product. */
        if ((i + 1) % 3 == 0)
            result = seq_mul(result, G);

        /* Update fact to (i)! for the next iteration: fact *= i */
        fact = seq_mul(fact, (seq_t)i);
    }

    return result;
}

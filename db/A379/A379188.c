/*
 * A379188  G.f. A(x) satisfies A(x) = 1/((1 - x*A(x)^3) * (1 - x*A(x))^3).
 * https://oeis.org/A379188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=0..n} binomial(n+3*k+1,k) *
 *            binomial(4*n+5*k+2,n-k)/(n+3*k+1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

/* Compute the n-th term of OEIS A379188.  The function works for all
   builds; when seq_t is an unsigned __int128 or an arbitrary‑precision
   class the product is performed exactly.  When seq_t is a 64‑bit
   unsigned integer the product is first folded into an unsigned
   __int128 to avoid overflow before truncating back to seq_t. */
seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t k = 0; k <= n; ++k) {
        /* indices for the binomial coefficients */
        idx_t m1 = n + 3 * k + 1;        /* n + 3*k + 1  */
        idx_t m2 = 4 * n + 5 * k + 2;    /* 4*n + 5*k + 2 */

        /* binomial values */
        seq_t b1 = binomial(m1, k);
        seq_t b2 = binomial(m2, n - k);

        /* product b1*b2 divided by m1.
           Use unsigned __int128 to avoid overflow when seq_t is 64‑bit. */
        unsigned __int128 prod = (unsigned __int128)b1 * (unsigned __int128)b2;
        prod /= (unsigned __int128)m1;

        res += (seq_t)prod;
    }
    return res;
}

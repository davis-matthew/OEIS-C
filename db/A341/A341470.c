/*
 * A341470  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          T(n,k) = Sum_{j=0..n} binomial(k*n,n-j) * binomial(k*n+j,j).
 * https://oeis.org/A341470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th entry of the square array T(i,j) read by
 *            antidiagonals. The element T(i,j) is given by T(i,j) =
 *            Σ_{k=0..i} 2^k * binomial(i,k) * binomial(i*j,k) where
 *            binomial(a,b) is the usual nCk. The indices (i,j) are obtained
 *            from n by the standard antidiagonal enumeration: for a given n
 *            cho...
 * Range:     u8 n <= 17, u16 n <= 40, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"          /* provides seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    /*  Determine diagonal d and coordinates (i,j) of the n‑th element  */
    idx_t d = 0;
    while ( (d + 1) * (d + 2) / 2 <= n )
        ++d;                                 /* d ≈ ⌊(√(8n+1)-1)/2⌋ */

    idx_t i    = n - d * (d + 1) / 2;        /* row index */
    idx_t j    = d - i;                       /* column index */

    /*  Compute T(i,j) = Σ_{k=0..i} 2^k * binomial(i,k) * binomial(i*j,k)  */
    seq_t sum  = 0;
    seq_t pow2 = 1;
    idx_t prod = i * j;                      /* i*j fits easily into idx_t */

    for (idx_t k = 0; k <= i; ++k) {
        seq_t term = pow2 * binomial(i, k);
        term *= binomial(prod, k);
        sum += term;
        pow2 <<= 1;                           /* next power of two */
    }

    return sum;
}

/*
 * A357771  Number of 2n-step closed paths on quasi-regular rhombic
 *          (rhombille) lattice starting from a degree-6 node.
 * https://oeis.org/A357771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   a(n) = 2^n * Σ_{k=0..n} binomial(n,k)^2 * binomial(2k,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    seq_t binom_nk = 1;   /* binom(n,0) */
    seq_t cent = 1;       /* binom(0,0) = C(2*0,0) */

    for (idx_t k = 0; k <= n; ++k)
    {
        /* term = binom(n,k)^2 * binom(2k,k) */
        seq_t term = binom_nk * binom_nk; /* binom(n,k)^2 */
        term *= cent;                    /* multiply by C(2k,k) */
        sum += term;

        if (k < n)
        {
            /* update binom(n,k+1) */
            binom_nk = (binom_nk * (seq_t)(n - k)) / (k + 1);

            /* update C(2(k+1),(k+1)) from C(2k,k) */
            /* C(2(k+1),(k+1)) = C(2k,k) * [2*(2k+1)]/(k+1) */
            cent = (cent * (seq_t)(2 * (2 * k + 1))) / (k + 1);
        }
    }

    /* compute 2^n */
    seq_t pow2 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow2 <<= 1; /* left shift by 1 (multiply by 2) */

    return sum * pow2;
}

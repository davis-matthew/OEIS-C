/*
 * A396491  Number of unsatisfiable 3-SAT formulas with n variables and 5
 *          clauses in the multiset clause model.
 * https://oeis.org/A396491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*binomial(n,1) + 6720*binomial(n,2) +
 *            224102*binomial(n,3) + 1766568*binomial(n,4) +
 *            6055360*binomial(n,5) + 13099520*binomial(n,6) +
 *            17940160*binomial(n,7) + 15088640*binomial(n,8) +
 *            7096320*binomial(n,9) + 1433600*binomial(n,10) (offset 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 9, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* coefficients for binomial terms k=1..10 */
    static const seq_t coeff[10] = {
        20ULL,
        6720ULL,
        224102ULL,
        1766568ULL,
        6055360ULL,
        13099520ULL,
        17940160ULL,
        15088640ULL,
        7096320ULL,
        1433600ULL
    };
    seq_t result = 0;
    for (idx_t k = 1; k <= 10; ++k)
    {
        seq_t bin = binomial(n, k);
        seq_t term = coeff[k-1] * bin;
        result += term;
    }
    return result;
}

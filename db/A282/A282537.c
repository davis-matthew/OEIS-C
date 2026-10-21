/*
 * A282537  Coefficients of the '5th-order' mock theta function Psi(q) with
 *          a(0)=1.
 * https://oeis.org/A282537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 182, u32 n <= 560, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = Σ_{k≥0} p_k(n – 5k^2),
 * where p_k(m) is the number of partitions of m into parts that are ≤ 5k+2
 * and congruent to 2 or 3 modulo 5.  This implements OEIS A282537.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate array of coefficients up to n */
    size_t len = (size_t)n + 1;
    seq_t *coeff = (seq_t *)malloc(len * sizeof(seq_t));
    if (!coeff)
        return 0;               /* allocation failure (should not happen) */

    /* initialise */
    for (size_t i = 0; i < len; ++i)
        coeff[i] = 0;
    coeff[0] = 1;

    /* add part 2 (present for k = 0) */
    if (n >= 2) {
        for (idx_t i = 2; i <= n; ++i)
            coeff[i] += coeff[i - 2];
    }

    seq_t result = 0;

    for (idx_t k = 0; ; ++k) {
        /* shift = 5*k^2 (may overflow, use 128-bit) */
        unsigned __int128 temp = (unsigned __int128)k * k * 5;
        if (temp > n)
            break;
        idx_t shift = (idx_t)temp;

        if (shift <= n)
            result += coeff[n - shift];

        /* add new parts for the next k:
           partA = 5*k + 3   (congruent to 3 mod 5)
           partB = 5*(k+1)+2 (congruent to 2 mod 5)
        */
        idx_t partA = 5 * k + 3;
        idx_t partB = 5 * (k + 1) + 2;

        if (partA <= n) {
            for (idx_t i = partA; i <= n; ++i)
                coeff[i] += coeff[i - partA];
        }
        if (partB <= n) {
            for (idx_t i = partB; i <= n; ++i)
                coeff[i] += coeff[i - partB];
        }
    }

    free(coeff);
    return result;
}

/*
 * A395571  Triangle read by rows: coefficients of reciprocal polynomials
 *          refining A007334.
 * https://oeis.org/A395571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = row n of triangle of numerators of the generating
 *            functions Sum_{k>=0} p_n(k) t^k, where p_n(x) = [ ∏_{i=2}^{n}
 *            ((n+2)x + i) * (2x+1) ] / n!. The answer for each flattened
 *            index n (starting at 0) is the coefficient of t^{k} in the
 *            numerator for the corresponding row. The algorithm: 1. Dete...
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/factorial.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Step 1: find row r */
    idx_t r = 0;
    while ( ( (idx_t)(r + 1) * (r + 2) ) / 2 <= n )
        ++r;
    /* Now r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t start = (idx_t)r * (r + 1) / 2;
    idx_t k = n - start;          /* offset within the row */

    /* Step 2: compute factorial(r) */
    seq_t fact_r = factorial(r);   /* factorial helper */

    /* Step 3: compute a_j for j = 0..r */
    /* Allocate array with stack space because r <= ~150 for n<=10000 */
    seq_t aValues[150];            /* 150 > sqrt(2*10^4) ≈ 142 */
    for (idx_t j = 0; j <= r; ++j) {
        seq_t prod = 1;
        for (idx_t i = 2; i <= r; ++i) {
            seq_t term = (seq_t)( (r + 2) * j + i );
            prod *= term;
        }
        prod *= (seq_t)( 2 * j + 1 );   /* multiply by (2j+1) */
        aValues[j] = prod / fact_r;     /* exact division */
    }

    /* Step 4: compute the numerator coefficient for offset k */
    seq_t result = 0;
    for (idx_t j = 0; j <= r + 1 && j <= k; ++j) {
        seq_t bin = binomial( r + 1, j );   /* C(r+1, j) */
        seq_t term = bin * aValues[ k - j ];
        if ( j & 1 )
            result -= term;
        else
            result += term;
    }
    return result;
}

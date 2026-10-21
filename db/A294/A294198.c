/*
 * A294198  Labeled trees on n nodes with at least one node of degree two.
 * https://oeis.org/A294198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-2)! * Sum_{q=1..n-2}
 *            C(n,q)*(-1)^(q+1)*(n-q)^(n-2-q)/(n-2-q)!
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n <= 2) return 0;

    /* positive and negative sums of the alternating inclusion–exclusion terms */
    seq_t pos = 0, neg = 0;

    /* C(n,q) initialised to C(n,1) = n */
    seq_t comb = (seq_t)n;
    /* (n-2)!/(n-2-q)! initial value for q=1  ==> (n-2) */
    seq_t mult = (seq_t)(n - 2);

    for (idx_t q = 1; q <= n - 2; ++q) {
        /* (n-q)^(n-2-q) */
        seq_t pw = big_pow((seq_t)(n - q), (seq_t)(n - 2 - q));

        /* term = C(n,q) * (n-q)^(n-2-q) * (n-2)!/(n-2-q)! */
        seq_t term = comb;
        term *= pw;
        term *= mult;

        if (q & 1ULL) /* q is odd => (-1)^(q+1) = +1 */
            pos += term;
        else
            neg += term;

        /* Prepare for next q if any */
        if (q < n - 2) {
            /* C(n,q+1) = C(n,q) * (n-q)/(q+1) */
            comb = (comb * (seq_t)(n - q)) / (seq_t)(q + 1);
            /* mult_{q+1} = mult_q * (n-2-q) */
            mult *= (seq_t)(n - 2 - q);
        }
    }

    /* a(n) = pos - neg (pos ≥ neg for all n ≥ 3) */
    seq_t result = pos;
    result -= neg;
    return result;
}

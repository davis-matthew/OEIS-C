/*
 * A393295  Number of transitive relations on an n-set with at most three
 *          ordered pairs.
 * https://oeis.org/A393295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            C(n,0)+C(n,1)+9*C(n,2)+49*C(n,3)+128*C(n,4)+180*C(n,5)+120*C(n,6)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          = (1/6)*(n^6 - 6*n^5 + 27*n^4 - 53*n^3 + 56*n^2 - 19*n + 6) */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    static const seq_t coeffs[7] = {
        (seq_t)1,   /* C(n,0) */
        (seq_t)1,   /* C(n,1) */
        (seq_t)9,   /* C(n,2) */
        (seq_t)49,  /* C(n,3) */
        (seq_t)128, /* C(n,4) */
        (seq_t)180, /* C(n,5) */
        (seq_t)120  /* C(n,6) */
    };
    seq_t result = (seq_t)0;
    /* Only evaluate terms up to min(n,6) */
    for (idx_t k = 0; k <= 6; ++k) {
        if (k > n) break;
        seq_t term = binomial(n, k);   /* binomial from utils */
        term *= coeffs[k];
        result += term;
    }
    return result;
}

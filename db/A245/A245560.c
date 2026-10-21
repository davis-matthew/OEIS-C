/*
 * A245560  Row sums of triangle in A144480.
 * https://oeis.org/A245560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+2)*2^(n-1) - (n/2)*binomial(n,n/2) for even n; a(n) =
 *            (n+2)*2^(n-1) - ((n+1)/4)*binomial(n+1,(n+1)/2) for odd n
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;

    /* term1 = (n+2) * 2^(n-1) */
    seq_t pow2_val = pow2(n - 1);                 /* 2^(n-1) */
    seq_t coeff = (seq_t)(n + 2);                 /* n+2 */
    seq_t term1 = coeff * pow2_val;               /* term1 */

    /* term2 depends on parity of n */
    seq_t term2;
    if ((n & 1) == 0) {                          /* n even */
        seq_t half = (seq_t)(n / 2);
        seq_t bin = binomial(n, n / 2);
        term2 = half * bin;
    } else {                                      /* n odd */
        seq_t mul = (seq_t)(n + 1) * binomial(n + 1, (n + 1) / 2);
        term2 = mul / (seq_t)4;                  /* exact division by 4 */
    }

    return term1 - term2;
}

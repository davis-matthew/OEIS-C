/*
 * A262739  O.g.f. exp( Sum_{n >= 1} A262733(n)*x^n/n ).
 * https://oeis.org/A262739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 20,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(0) = 1; for n ≥ 1,
 *           a(n) = 1/(n+1) * Σ_{i=0..n} binomial(7*(n+1),i)*
 *                                 binomial(6*(n+1)-i-2,(n+1)-i-1)
 *
 * Implementation uses iterative computation of the two binomial factors
 * to obtain an O(n) algorithm that works for any supported seq_t type.
 */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* Here N = n + 1 is the parameter used in the summation */
    idx_t N = n + 1;
    seq_t sum = (seq_t)0;

    /* bin1_i = C(7N, i);  start with i = 0 */
    seq_t bin1 = (seq_t)1;

    /* bin2_i = C(6N-i-2, N-i-1);  start with i = 0 */
    seq_t bin2 = binomial((idx_t)(6 * N - 2), (idx_t)(N - 1));

    for (idx_t i = 0; i < N; ++i) {
        /* accumulate term bin1 * bin2 */
        sum += bin1 * bin2;

        /* No need to update after the last iteration */
        if (i + 1 == N) {
            break;
        }

        /* update bin1_{i+1} = bin1 * (7N - i) / (i + 1) */
        bin1 = bin1 * (seq_t)(7 * N - i) / (seq_t)(i + 1);

        /* update bin2_{i+1} = bin2 * (N - i - 1) / (6N - i - 2) */
        bin2 = bin2 * (seq_t)(N - i - 1) / (seq_t)(6 * N - i - 2);
    }

    /* a(n) = sum / N */
    return sum / (seq_t)N;
}

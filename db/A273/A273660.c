/*
 * A273660  Number of forests of labeled rooted trees of height at most 1,
 *          with n labels, ten of which are used for root nodes and any root
 *          may contain >= 1 labels.
 * https://oeis.org/A273660
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(n,10) * Sum_{j=1..10} Stirling2(10,j) * j^(n-10)
 * Range:     u8 none, u16 none, u32 n <= 12, u64 n <= 21, u128 n <= 39,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

/* Stirling numbers S(10,j) for j=0..10 */
static const int STIRLING_INT[11] = {
    0,  1,  511, 9330, 34105, 42525, 22827, 5880, 750, 45, 1
};

seq_t a(idx_t n)
{
    if (n < 10)
        return 0;                          /* undefined for n < 10 */

    seq_t binom = binomial(n, 10);
    seq_t exp   = (seq_t)(n - 10);

    seq_t sum = 0;
    for (idx_t j = 1; j <= 10; ++j) {
        seq_t powj = big_pow((seq_t)j, exp);          /* j^(n-10) */
        sum += (seq_t)STIRLING_INT[j] * powj;
    }

    return binom * sum;
}

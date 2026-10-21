/*
 * A273657  Number of forests of labeled rooted trees of height at most 1,
 *          with n labels, seven of which are used for root nodes and any root
 *          may contain >= 1 labels.
 * https://oeis.org/A273657
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(n,7) * Sum_{j=0..7} Stirling2(7,j) * j^(n-7)
 * Range:     u8 none, u16 n <= 8, u32 n <= 12, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Values are defined only for n >= 7 (offset 7) */
    if (n < 7)
        return 0;

    /* Stirling numbers of the second kind for 0 ≤ j ≤ 7 */
    static const unsigned long long S[8] = {0, 1, 63, 301, 350, 140, 21, 1};

    /* Compute C(n,7) multiplicatively: avoid factorials. */
    seq_t comb = 1;
    for (idx_t k = 0; k < 7; ++k)
    {
        comb = comb * (seq_t)(n - k);
        comb = comb / (seq_t)(k + 1);
    }

    seq_t sum = 0;
    for (idx_t j = 1; j <= 7; ++j)
    {
        seq_t pow   = seq_pow((seq_t)j, n - 7);
        seq_t term  = (seq_t)S[j] * pow;
        sum += term;
    }

    return comb * sum;
}

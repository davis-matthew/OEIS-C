/*
 * A273655  Number of forests of labeled rooted trees of height at most 1,
 *          with n labels, five of which are used for root nodes and any root
 *          may contain >= 1 labels.
 * https://oeis.org/A273655
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binom(n,5) * Σ_{j=0..5} Stirling2(5,j) * j^(n-5)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   binomial, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"   /* binomial(idx_t a, idx_t b) */
#include "../utils/seq_pow.h"    /* seq_pow(seq_t base, idx_t exp) */

seq_t a(idx_t n)
{
    if (n < 5)
        return 0;

    /* Stirling numbers of the second kind for 5: S(5,0)=0, S(5,1)=1,
       S(5,2)=15, S(5,3)=25, S(5,4)=10, S(5,5)=1 */
    static const seq_t ST[6] = {0ULL, 1ULL, 15ULL, 25ULL, 10ULL, 1ULL};

    seq_t binomC = binomial(n, 5);
    seq_t sum = 0;
    idx_t exp = n - 5;

    for (idx_t j = 1; j <= 5; ++j) {
        seq_t pow = seq_pow((seq_t)j, exp);   /* j^(n-5) */
        sum += pow * ST[(size_t)j];
    }

    return binomC * sum;
}

/*
 * A273656  Number of forests of labeled rooted trees of height at most 1,
 *          with n labels, six of which are used for root nodes and any root
 *          may contain >= 1 labels.
 * https://oeis.org/A273656
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binom(n,6) * Sum_{j=0..6} Stirling2(6,j) * j^(n-6)
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t STIRLING[7] = {
    (seq_t)0, (seq_t)1, (seq_t)31, (seq_t)90,
    (seq_t)65, (seq_t)15, (seq_t)1
};

static seq_t pow_int(idx_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t b = (seq_t)base;
    while (exp)
    {
        if (exp & 1u)
            result = result * b;
        b = b * b;
        exp >>= 1u;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 6u)
        return (seq_t)0;
    /* Compute binom(n,6) via successive multiplication/division */
    seq_t binom = (seq_t)1;
    for (idx_t i = 1; i <= 6; ++i)
    {
        binom = binom * (n - 6u + i);
        binom = binom / i;
    }
    /* Compute the sum j^(n-6) weighted by Stirling numbers */
    idx_t e = n - 6u;
    seq_t sum = (seq_t)0;
    for (idx_t j = 1; j <= 6; ++j)
    {
        seq_t p = pow_int(j, e);
        sum += STIRLING[j] * p;
    }
    return binom * sum;
}

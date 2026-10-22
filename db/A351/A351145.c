/*
 * A351145  Triangle T(n,m) read by rows: Sum_{k=1..m} binomial(2*n,
 *          n+k)*d(k), m<=n, with d(k)=A000005(k).
 * https://oeis.org/A351145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,m) = \sum_{k=1}^{m} binomial(2*n, n+k) * d(k) where d(k) is
 *            the number of divisors of k (A000005). The sequence a(n) is the
 *            triangle read by rows: a(1)=T(1,1), a(2)=T(2,1), a(3)=T(2,2),
 *            a(4)=T(3,1), … Offset = 1
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>            /* for uint64_t, provided by the build */
#include "../utils/binom.h"    /* provides seq_t binom(idx_t a, idx_t b) */

/* number of positive divisors of a small integer k (k <= ~200) */
static seq_t divisor_count(idx_t k)
{
    seq_t cnt = 1;
    idx_t tmp = k;
    for (idx_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            idx_t exp = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++exp;
            }
            cnt *= (seq_t)(exp + 1);
        }
    }
    if (tmp > 1)          /* remaining prime factor */
        cnt *= 2;
    return cnt;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; a(1) is the first term */
        return 0;

    /* Find the row r such that r*(r+1)/2 >= n. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* Compute column index m within that row. */
    idx_t prev_terms = (r - 1) * r / 2;   /* terms before row r */
    idx_t m = n - prev_terms;             /* 1 <= m <= r */

    seq_t sum = 0;
    for (idx_t k = 1; k <= m; ++k)
    {
        seq_t dk  = divisor_count(k);
        seq_t bin = binom(2 * r, r + k);   /* helper from utils */
        sum += bin * dk;
    }
    return sum;
}

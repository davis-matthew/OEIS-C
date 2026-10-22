/*
 * A304429  Triangle read by rows: T(n,k) (for 0 <= k <= floor(n/2)) is the
 *          number of permutations of length n that have k descents and avoid
 *          the patterns 321 and 2341.
 * https://oeis.org/A304429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the nth element of the flattened triangle T(n,k) defined
 *            by: T(n,k)=0 for k>floor(n/2); T(n,0)=1; for k>0,
 *            T(n,k)=∑_{j=0..n-2k}
 *            (-1)^{n-j}·2^{3k-n+j-1}·C(3k+j,3k)·C(k-1,n-2k-j)
 * Range:     u8 n <= 21, u16 n <= 51, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

static inline seq_t perm_T(idx_t n, idx_t k)
{
    if (k == 0)
        return (seq_t)1;
    seq_t pos_sum = (seq_t)0;
    seq_t neg_sum = (seq_t)0;
    idx_t max_j = n - 2 * k;
    for (idx_t j = 0; j <= max_j; ++j) {
        seq_t term = binomial(3 * k + j, 3 * k);
        term *= binomial(k - 1, max_j - j);
        /* exponent may be negative */
        int64_t exp = (int64_t)3 * (int64_t)k - (int64_t)n + (int64_t)j - 1;
        if (exp >= 0)
            term *= pow2((idx_t)exp);
        else
            term /= pow2((idx_t)(-exp));
        /* sign */
        if ( ((n - j) & 1) == 0 )
            pos_sum += term;
        else
            neg_sum += term;
    }
    return pos_sum - neg_sum;  /* pos_sum >= neg_sum */
}

seq_t a(idx_t n)
{
    /* Find row r such that cumulative count up to r-1 <= n < up to r */
    idx_t r = 0;
    idx_t cum = 0;
    while (true) {
        idx_t len = (r >> 1) + 1;   /* floor(r/2)+1 */
        if (n < cum + len) {
            idx_t k = n - cum;      /* position in row r */
            return perm_T(r, k);
        }
        cum += len;
        ++r;
    }
}

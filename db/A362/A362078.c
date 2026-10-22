/*
 * A362078  Square array T(n,k), n >= 0, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) = [x^n] 1/(1 - x*(1+x)^k)^n.
 * https://oeis.org/A362078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the term obtained by reading the square array T(i,k)
 *            along the antidiagonals. T(i,k) = Σ_{j=0..i} binomial(i+j-1, j)
 *            * binomial(k*j, i-j), with T(0,k) = 1 for all k. For a given n,
 *            let d be the smallest integer with (d+1)(d+2)/2 > n. Then offset
 *            = n - d(d+1)/2, and a(n) = T(offset, d-offset)
 * Range:     u8 n <= 19, u16 n <= 53, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t N)
{
    /* Find diagonal d: minimal d such that (d+1)(d+2)/2 > N */
    idx_t low = 0, high = N;                 /* high never overflow */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 cum = (__int128)(mid + 1) * (mid + 2) / 2;
        if (cum > (unsigned __int128)N)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t d = low;                           /* (d+1)(d+2)/2 > N */

    /* offset within this diagonal */
    __int128 cum_prev = (__int128)d * (d + 1) / 2;  /* terms before diagonal d */
    idx_t offset = (idx_t)((unsigned __int128)N - cum_prev);
    idx_t i = offset;                       /* row index in T */
    idx_t k = d - i;                         /* column index in T */

    if (i == 0)                 /* T(0,k) = 1 for all k */
        return (seq_t)1u;

    seq_t val = 0;
    for (idx_t j = 1; j <= i; ++j) {
        seq_t c1 = binomial(i + j - 1, j);      /* binomial(i+j-1, j) */
        seq_t c2 = binomial(k * j, i - j);     /* binomial(k*j, i-j) */
        val += c1 * c2;
    }
    return val;
}

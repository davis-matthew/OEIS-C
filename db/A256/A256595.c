/*
 * A256595  Triangle A074909(n) with 0's as second column.
 * https://oeis.org/A256595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) = 1 if c = 0 * a(n) = 0 if c = 1 * a(n) = binomial(r+1,
 *            c) otherwise * * where r = floor((sqrt(8*n+1)-1)/2) (i.e. the
 *            largest r with * r*(r+1)/2 <= n) and c = n - r*(r+1)/2. *
 *            binomial(n,k) is computed multiplicatively
 * Range:     u8 n <= 58, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
/* seq_t and idx_t are defined by the build */

static seq_t _binom(seq_t N, idx_t k)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (N - (seq_t)(i - 1)) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n) {
    /* Find r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;

    idx_t prev = r * (r + 1) / 2;
    idx_t c = n - prev;            /* column within the row */

    if (c == 0) return (seq_t)1;
    if (c == 1) return (seq_t)0;

    /* For c >= 2 we need binomial(r+1, c) */
    return _binom((seq_t)(r + 1), c);
}

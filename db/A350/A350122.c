/*
 * A350122  Square array T(n,k), n >= 1, k >= 1, read by antidiagonals
 *          downwards, where T(n,k) = Sum_{j=1..n} floor(n/(2*j-1))^k.
 * https://oeis.org/A350122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where T(r,k) = Σ_{j=1..r} floor(r/(2*j-1))^k and
 *            (r,k) are obtained by reading the square array T(n,k) by
 *            antidiagonals downwards. For an index n ≥ 1, let t be the
 *            smallest integer such that t*(t+1)/2 ≥ n. Then the offset inside
 *            this diagonal is p = n − (t-1)*t/2 and we have r = p,...
 * Range:     u8 n <= 24, u16 n <= 58, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t
pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;        /* undefined below offset, return 0 */

    /* find diagonal t such that t*(t+1)/2 >= n */
    idx_t t = 1;
    idx_t tri = 1;          /* triangular number t*(t+1)/2 */
    while (tri < n) {
        ++t;
        tri += t;
    }

    /* offset p in this diagonal */
    idx_t prev = tri - t;   /* (t-1)*t/2 */
    idx_t p = n - prev;     /* 1-based position within the diagonal */

    idx_t r = p;            /* row index (corresponds to n in T(n,k)) */
    idx_t k = t - p + 1;    /* column index (corresponds to k in T(n,k)) */

    /* compute T(r,k) = Σ floor(r/(2*j-1))^k */
    seq_t sum = 0;
    for (idx_t j = 1; j <= r; ++j) {
        idx_t denom = 2 * j - 1;
        idx_t q = r / denom;  /* floor division */

        if (q == 0)
            continue;        /* contributes 0 */

        seq_t term = pow_seq((seq_t)q, k);
        sum += term;
    }

    return sum;
}

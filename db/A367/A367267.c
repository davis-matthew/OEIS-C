/*
 * A367267  Triangle read by rows. T(n, k) = binomial(n, k) * binomial(n - 1,
 *          k - 1).
 * https://oeis.org/A367267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th term of the triangle read by rows, where T(n,k)
 *            = C(n,k) * C(n-1,k-1). Thus a(n) = T(r,k) with r such that
 *            r*(r+1)/2 <= n < (r+1)*(r+2)/2 and k = n - r*(r+1)/2. This uses
 *            the binomial helper implemented multiplicatively to avoid large
 *            intermediate values
 * Range:     u8 n <= 30, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* multiplicative binomial coefficient: C(n,k) for idx_t arguments.
   It works for all n,k that fit in idx_t and returns a seq_t. */
static seq_t choose(idx_t n, idx_t k)
{
    if (k > n)
        return 0;
    if (k == 0 || k == n)
        return 1;
    if (k > n - k)          /* take advantage of symmetry */
        k = n - k;

    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* Multiply by (n - k + i) first, then divide by i.
           All operations are on seq_t, so no overflow in intermediate
           idx_t values. */
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column index within the row. */
    idx_t k = n - r * (r + 1) / 2;

    /* Special cases at the start of a row. */
    if (r == 0 && k == 0)
        return 1;          /* T(0,0) */
    if (k == 0)
        return 0;          /* first element of any row n>0 */

    /* Compute T(r,k) = C(r,k) * C(r-1,k-1). */
    seq_t c1 = choose(r, k);
    seq_t c2 = choose(r - 1, k - 1);
    return c1 * c2;
}

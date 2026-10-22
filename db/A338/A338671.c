/*
 * A338671  a(n) is the number of distinct ways of arranging n identical
 *          square tiles into two rectangles.
 * https://oeis.org/A338671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{a=1}^{⌊(n‑1)/2⌋} r(a)·r(n‑a) * + (n even ?
 *            r(n/2)·(r(n/2)+1)/2 : 0) * where r(k) = (τ(k)+1)/2 and τ(k) is
 *            the number of divisors of k
 * Range:     u8 n <= 83, u16 n <= 4919, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* defined for n≥1 only */

    idx_t N = n;

    /* divisor count array d[0..N] */
    uint32_t *d = (uint32_t *)malloc((N + 1) * sizeof(uint32_t));
    if (!d)
        return 0;
    for (idx_t i = 0; i <= N; ++i)
        d[i] = 0;

    for (idx_t i = 1; i <= N; ++i)
        for (idx_t j = i; j <= N; j += i)
            ++d[j];

    /* r[0..N] = (d[i]+1)/2  as seq_t */
    seq_t *r = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!r) {
        free(d);
        return 0;
    }
    for (idx_t i = 0; i <= N; ++i)
        r[i] = ((seq_t)d[i] + (seq_t)1) / (seq_t)2;

    seq_t ans = (seq_t)0;

    idx_t limit = (N - 1) / 2;          /* a < n-a */
    for (idx_t a = 1; a <= limit; ++a)
        ans += r[a] * r[N - a];

    if ((N & (idx_t)1) == 0) {          /* n even: a = b case */
        seq_t t = r[N / 2];
        ans += t * (t + (seq_t)1) / (seq_t)2;
    }

    free(d);
    free(r);
    return ans;
}

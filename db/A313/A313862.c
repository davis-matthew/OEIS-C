/*
 * A313862  Coordination sequence Gal.6.354.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 Let d(n) be the difference a(n)-a(n-1). d(1)=4, d(2)=5,
 *            d(3)=6, d(4)=5, d(5)=4, d(6)=6, d(7)=6. For n ≥ 8: d(n) = d(n-6)
 *            + ( -1 if n % 4 ≤ 1 else +1 ) Then a(n) = a(n-1) + d(n)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;

    /* allocate array for d[0..n]  */
    size_t sz = (size_t)n + 1;
    seq_t *d = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!d) return 1;               /* memory error – return minimal value */

    /* base differences 1..7  */
    static const seq_t base[8] = {0, 4, 5, 6, 5, 4, 6, 6};
    for (idx_t i = 1; i <= 7 && i <= n; ++i) d[i] = base[i];

    /* compute differences for i ≥ 8  */
    for (idx_t i = 8; i <= n; ++i) {
        int s = (i % 4 <= 1) ? -1 : 1;
        if (s == 1)
            d[i] = d[i - 6] + 1;
        else
            d[i] = d[i - 6] - 1;      /* d[i] > 0 for all i */
    }

    /* build the sequence values  */
    seq_t ans = 1;
    for (idx_t i = 1; i <= n; ++i)
        ans += d[i];

    free(d);
    return ans;
}

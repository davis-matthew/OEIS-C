/*
 * A316064  Coordination sequence Gal.6.670.4 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A316064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Coordination sequence Gal.6.670.4
   a(0) = 1
   a(1) = 6
   a(2) = 9
   a(3) = 16
   a(4) = 19
   For n ≥ 5: a(n) = 5*n + d
   Let k = n / 5, r = n % 5.
   Then d is given by
        r = 0: d = 3*k  (k>0); d = 1  (k=0)
        r = 1: d = k+1
        r = 2: d = k   (k>0)   (k=0 only at n=2, handled separately)
        r = 3: d = k+2 (k>0); d = 1  (k=0)
        r = 4: d = k-1 (k>0); d = -1 (k=0)
*/

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the only special negative-difﬃrent case. */
    if (n == 2u) return 9u;

    idx_t k = n / 5u;
    idx_t r = n % 5u;

    seq_t ans = (seq_t)5 * n;   /* 5*n in the type of the sequence */
    long long diff;             /* signed intermediate so we can subtract 1 */

    switch (r)
    {
        case 0u:  /* r = 0 */
            diff = (k == 0u) ? 1 : 3LL * (long long)k;
            break;
        case 1u:  /* r = 1 */
            diff = 1 + (long long)k;
            break;
        case 2u:  /* r = 2, k must be >0 because n!=2 */
            diff = (long long)k;
            break;
        case 3u:  /* r = 3 */
            diff = (k == 0u) ? 1 : (long long)k + 2;
            break;
        case 4u:  /* r = 4 */
            diff = (k == 0u) ? -1 : (long long)k - 1;
            break;
        default:
            diff = 0;  /* unreachable */
            break;
    }

    if (diff >= 0)
        ans += (seq_t)diff;
    else
        ans -= (seq_t)(-diff);

    return ans;
}

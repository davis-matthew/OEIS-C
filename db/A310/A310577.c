/*
 * A310577  Coordination sequence Gal.6.42.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = if n < 10: a(n) = base[n] (base =
 *            {1,4,6,10,12,16,18,22,24,28}) else: let r = n mod 10, k = n / 10
 *            if r == 0: a = 32*k - 2 if r == 1: a = 6 + 40*((n-1)/10) if r ==
 *            2: a = 6 + 32*((n-2)/10) else: a = base[r] + 32*((n-r)/10)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const seq_t base[10] = {1,4,6,10,12,16,18,22,24,28};

    if (n < 10)
        return base[n];

    idx_t r = n % 10;
    seq_t res;

    if (r == 0) {
        /* n = 10k, k >= 1 */
        idx_t k = n / 10;
        res = (seq_t)32 * (seq_t)k - (seq_t)2;
    } else if (r == 1) {
        /* n = 10k+1, k >= 1 (n >= 11) */
        idx_t k = (n - 1) / 10;
        res = (seq_t)6 + (seq_t)40 * (seq_t)k;
    } else if (r == 2) {
        /* n = 10k+2, k >= 0 (n >= 2) */
        idx_t k = (n - 2) / 10;
        res = (seq_t)6 + (seq_t)32 * (seq_t)k;
    } else {
        /* r in {3,4,5,6,7,8,9} */
        idx_t k = (n - r) / 10;
        res = base[r] + (seq_t)32 * (seq_t)k;
    }

    return res;
}

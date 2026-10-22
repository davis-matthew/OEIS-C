/*
 * A310524  Coordination sequence Gal.6.356.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + 2*floor(n/6) + C(r,k mod 2) where r = n mod 6, k =
 *            floor(n/6) and C(r,parity) = { r = 0 -> 0 r = 1 -> -1 if k even,
 *            else 0 r = 2 -> 0 r = 3 -> +1 r = 4 -> +2 r = 5 -> +2 if k even,
 *            else +3 } (a(0) = 1 by definition)
 * Range:     u8 n <= 47, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t k = n / 6;           /* floor(n/6)          */
    idx_t r = n % 6;           /* n mod 6             */
    bool even_k = ((k & 1) == 0);

    /* base: 5*n + 2*k */
    seq_t res = (seq_t)n * 5 + (seq_t)(2 * k);

    /* add correction depending on r and parity of k */
    switch (r) {
        case 1:
            if (even_k)
                res -= (seq_t)1;
            break;
        case 3:
            res += (seq_t)1;
            break;
        case 4:
            res += (seq_t)2;
            break;
        case 5:
            res += even_k ? (seq_t)2 : (seq_t)3;
            break;
        /* cases 0 and 2 add nothing */
        default:
            break;
    }

    return res;
}

/*
 * A315899  Coordination sequence Gal.6.582.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*n + offset(n) where offset(n) is * (n even): * r = n %
 *            8 * r==0: offset = (n==0?1:4*(n/8)) * r==2: offset = (n<10?0 :
 *            -4*((n-10)/8)) * r==4: offset = 2 - 4*((n-4)/8) * r==6: offset =
 *            -4*((n-6)/8) * (n odd): * n%4==3: offset = 2*n-4 * n%4==1: let
 *            k=(n-1)/4; * if k even offset = 8*k+2; els...
 * Range:     u8 n <= 44, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    int64_t offset = 0;

    if ((n & 1ULL) == 0) {            /* even n */
        switch (n % 8ULL) {
            case 0ULL:
                if (n == 0ULL)
                    offset = 1;
                else
                    offset = 4 * (n / 8ULL);
                break;
            case 2ULL: {
                if (n < 10ULL)
                    offset = 0;
                else
                    offset = -4 * ((n - 10ULL) / 8ULL);
                break;
            }
            case 4ULL:
                offset = 2 - 4 * ((n - 4ULL) / 8ULL);
                break;
            case 6ULL:
                offset = -4 * ((n - 6ULL) / 8ULL);
                break;
            default:
                offset = 0;   /* unreachable */
        }
    } else {                           /* odd n */
        if ((n % 4ULL) == 3ULL) {
            offset = 2 * (int64_t)n - 4;
        } else {                       /* n % 4 == 1 */
            int64_t k = (n - 1ULL) / 4ULL;
            if ((k & 1ULL) == 0)
                offset = 8 * k + 2;
            else
                offset = 8 * k - 2;
        }
    }

    seq_t res = (seq_t)4 * (seq_t)n;
    if (offset >= 0)
        res += (seq_t)offset;
    else
        res -= (seq_t)(-offset);
    return res;
}

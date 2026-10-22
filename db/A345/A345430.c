/*
 * A345430  For 1<=x<=n, 1<=y<=n, with gcd(x,y)=1, write 1 = gcd(x,y) =
 *          u*x+v*y with u,v minimal; a(n) = sum of the values of |v|.
 * https://oeis.org/A345430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{1<=x<=n,1<=y<=n,gcd(x,y)=1} |v_min(x,y)|, where
 *            (u,v) satisfies u*x + v*y = 1 and minimizes u^2 + v^2. For x=y=1
 *            we use (u,v) = (0,1)
 * Range:     u8 n <= 14, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   modinv_idx_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/modinv_idx_t.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t sum = 0;
    /* special case (1,1) */
    if (n >= 1) sum += 1;

    for (idx_t x = 2; x <= n; ++x) {
        for (idx_t r = 1; r < x && r <= n; ++r) {
            idx_t inv = modinv_idx_t(r, x);
            if (inv == 0) continue;      /* gcd(x,r) != 1 */

            idx_t cnt = (n >= r) ? ((n - r) / x + 1) : 0;
            idx_t m = inv;
            if (x - inv < m) m = x - inv;
            sum += (seq_t)cnt * m;
        }
    }
    return sum;
}

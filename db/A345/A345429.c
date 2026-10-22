/*
 * A345429  For 1<=x<=n, 1<=y<=n, with gcd(x,y)=1, write 1 = gcd(x,y) =
 *          u*x+v*y with u,v minimal; a(n) = sum of the values of |u|.
 * https://oeis.org/A345429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{1<=x<=n,1<=y<=n,gcd(x,y)=1} |u| where u is the
 *            minimal integer satisfying 1 = u*x + v*y
 * Range:     u8 n <= 14, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   mod_inv_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For x=y=1 we set u=0.  For y=1, u=0.  For y>1, |u| is the smallest
   absolute value of u in all solutions of a*u ≡ 1 (mod b), which is
   min(inv, b - inv) where inv is the modular inverse of a mod b. */

#include <stdint.h>
#include "../utils/mod_inv_u64.h"

/* fast gcd for unsigned integers */
static inline idx_t gcd_u64(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    seq_t sum = 0;

    for (idx_t b = 2; b <= n; ++b) {
        /* skip y==1 because all contributions are 0 */
        for (idx_t r = 1; r < b; ++r) {
            if (gcd_u64(r, b) != 1) continue;

            /* minimal |u| for this residue */
            seq_t inv = mod_inv_u64(r, b);           /* 1 <= inv <= b-1 */
            seq_t val = inv;
            seq_t alt = (seq_t)b - inv;
            if (alt < val) val = alt;

            /* count how many a in [1..n] are congruent to r (mod b) */
            idx_t cnt = 0;
            if (r <= n) {
                cnt = (n - r) / b + 1;
            }
            sum += (seq_t)cnt * val;
        }
    }

    return sum;
}

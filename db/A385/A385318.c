/*
 * A385318  Number of nonnegative s < n such that s^s == (-s)^s (mod n).
 * https://oeis.org/A385318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(n/2) + #{ odd 0 < s < n | 2·s^s ≡ 0 (mod n) } (If n
 *            ≡ 0 mod 4 the extra set is empty; for n ≡ 2 mod 4 it is
 *            non‑empty when (n/2) | s^s for odd s.)
 * Range:     u8 n <= 484, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n=0, but safe */

    /* ceil(n/2) */
    idx_t half = (n >> 1) + (n & 1);
    seq_t ans  = (seq_t)half;

    /* For n divisible by 4, no odd solutions contribute. */
    if ((n & 3) == 0) return ans;

    /* For n odd (mod 2) we need s^s ≡ 0 (mod n).
       For n even with n%4==2 we need (n/2) | s^s.          */
    for (idx_t s = 1; s < n; s += 2) {
        seq_t pow_val = mod_pow((seq_t)s, s, (seq_t)n);

        if (n & 1) {                     /* n is odd */
            if (pow_val == 0) ans += (seq_t)1;
        } else {                         /* n % 4 == 2, so n = 2*m */
            idx_t m = n >> 1;            /* n/2 */
            if (pow_val % (seq_t)m == 0) ans += (seq_t)1;
        }
    }
    return ans;
}

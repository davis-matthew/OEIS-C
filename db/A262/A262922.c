/*
 * A262922  a(1)=1; for n>1, a(n) = a(n-1) + n + 2 if a(n-1) and n are
 *          coprime, otherwise a(n) = a(n-1)/gcd(a(n-1),n).
 * https://oeis.org/A262922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; for n > 1, * if gcd(a(n-1), n) == 1 then a(n) = a(n-1)
 *            + n + 2, * else a(n) = a(n-1) / gcd(a(n-1), n)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined, but safe fallback */
    seq_t cur = 1;                       /* a(1) */
    if (n == 1) return cur;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t g = gcd(cur, (seq_t)i);
        if (g == 1)
            cur = cur + (seq_t)i + 2;
        else
            cur = cur / g;
    }
    return cur;
}

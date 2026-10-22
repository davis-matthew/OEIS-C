/*
 * A338295  For n > 1, a(n) is the largest base b <= n such that the digits of
 *          n in base b contain the digit b-1; a(1) = 1.
 * https://oeis.org/A338295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (n+1)/lpf(n+1) when n+1 is composite. For prime n+1, a(n)
 *            is the largest base b ≤ n such that the base‑b expansion of n
 *            contains the digit b‑1 (computed by brute force)
 * Range:     u8 n <= 510, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   lpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/lpf.h"   /* provides: idx_t lpf(idx_t n) */

seq_t a(idx_t n)
{
    if (n <= 1)               /* a(1) = 1 */
        return (seq_t)n;

    idx_t m = n + 1;          /* n+1 */
    idx_t p = lpf(m);         /* least prime factor of m */

    if (p != m) {             /* m composite */
        idx_t res = m / p;    /* (n+1)/lpf(n+1) fits in idx_t */
        return (seq_t)res;
    }

    /* m is prime: search backwards for the largest b with digit b-1 present */
    for (idx_t b = n; b >= 2; --b) {
        idx_t tmp = n;
        bool ok = false;
        while (tmp) {
            if (tmp % b == b - 1) {
                ok = true;
                break;
            }
            tmp /= b;
        }
        if (ok)
            return (seq_t)b;
    }

    /* Should never reach here, but keep the compiler happy */
    return (seq_t)1;
}

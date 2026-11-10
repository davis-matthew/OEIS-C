/*
 * A345947  a(n) = gcd(A153151(n), A344875(n)).
 * https://oeis.org/A345947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(A153151(n), A344875(n)), where A153151(n) = 2n-1 if n
 *            is a power of 2 (n>=2), else n-1 (a(1)=1); A344875 is
 *            multiplicative with f(2^e) = 2^(e+1)-1, f(p^e) = p^e-1 for odd p
 * Range:     u8 n <= 255, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 113 terms through n = 9635 (2026-11-10)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

static seq_t A345947_rot(idx_t n)
{
    if (n < 2)
        return (seq_t)n;
    if ((n & (n - 1)) == 0)
        return (seq_t)n * 2 - 1;
    return (seq_t)n - 1;
}

static seq_t A345947_mult(idx_t n)
{
    seq_t ans = 1;
    uint64_t t = (uint64_t)n;
    uint64_t p;

    if (t > 0 && t % 2 == 0) {
        seq_t pw = 2;               /* 2^(e+1) */
        while (t % 2 == 0) {
            t /= 2;
            pw *= 2;
        }
        ans *= (pw - 1);
    }
    for (p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            seq_t pw = 1;
            while (t % p == 0) {
                t /= p;
                pw *= (seq_t)p;
            }
            ans *= (pw - 1);
        }
    }
    if (t > 1)
        ans *= ((seq_t)t - 1);
    return ans;
}

seq_t a(idx_t n)
{
    return gcd(A345947_rot(n), A345947_mult(n));
}

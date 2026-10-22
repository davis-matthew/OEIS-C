/*
 * A302985  Number of ordered pairs (x, y) of nonnegative integers such that n
 *          - 2^x - 3*2^y has the form u^2 + 2*v^2 with u and v integers.
 * https://oeis.org/A302985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(x,y) ≥ 0 : n - 2^x - 3·2^y = u^2 + 2·v^2 for some
 *            integers u,v}
 * Range:     u8 n <= 829, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

static bool is_representable(idx_t m)
{
    if (m == 0)
        return true;
    idx_t t = m;
    /* factor 2 */
    if (t % 2 == 0) {
        while (t % 2 == 0)
            t /= 2;
    }
    /* odd factors */
    for (idx_t p = 3; p <= t / p; p += 2) {
        if (t % p == 0) {
            idx_t exp = 0;
            do {
                t /= p;
                exp++;
            } while (t % p == 0);
            if ((p % 8 == 5 || p % 8 == 7) && (exp & 1))
                return false;
        }
    }
    if (t > 1) {                 /* remaining prime factor */
        if (t % 8 == 5 || t % 8 == 7)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t N = (seq_t)n;

    for (idx_t x = 0; ; ++x) {
        seq_t powx = pow2(x);
        if (powx > N)
            break;
        seq_t diff = N - powx;           /* N - 2^x */

        for (idx_t y = 0; ; ++y) {
            seq_t powy = (seq_t)3 * pow2(y);
            if (powy > diff)
                break;
            seq_t m = diff - powy;       /* n - 2^x - 3·2^y */

            if (is_representable((idx_t)m))
                count++;
        }
    }
    return count;
}

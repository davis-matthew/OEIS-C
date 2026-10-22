/*
 * A390665  The sum of the unitary divisors of the smallest cube divisible by
 *          n.
 * https://oeis.org/A390665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e || n} (1 + p^(e + ((3 - e % 3) % 3)))
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1553, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0, guard */
    uint64_t nn = (uint64_t)n;      /* local 64‑bit copy */
    seq_t res = (seq_t)1;

    for (uint64_t p = 2; p * p <= nn; ++p) {
        if (nn % p == 0) {
            uint64_t e = 0;
            while (nn % p == 0) {
                nn /= p;
                ++e;
            }
            uint64_t rem = e % 3;
            uint64_t e2 = e + ((3 - rem) % 3);    /* exponent in the smallest cube */
            seq_t term = pow_uint(p, (idx_t)e2);
            term += (seq_t)1;
            res *= term;
        }
    }

    if (nn > 1) {   /* remaining prime factor with exponent 1 */
        uint64_t e = 1;
        uint64_t rem = e % 3;
        uint64_t e2 = e + ((3 - rem) % 3);
        seq_t term = pow_uint(nn, (idx_t)e2);
        term += (seq_t)1;
        res *= term;
    }

    return res;
}

/*
 * A390371  a(n) is the largest bi-unitary divisor of n that is an
 *          exponentially odd number (A268335).
 * https://oeis.org/A390371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product over prime powers p^e of p^(e-1) if e even (and
 *            ≠2), p^e if e odd, and 1 if e = 2
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t res = 1;
    uint64_t tmp = (uint64_t)n;

    for (uint64_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            if (e == 2) {
                /* contribution is 1 – skip */
            } else {
                int exp = (e % 2 == 0) ? e - 1 : e;
                seq_t contrib = 1;
                for (int i = 0; i < exp; ++i) {
                    contrib *= (seq_t)p;
                }
                res *= contrib;
            }
        }
    }

    /* remaining prime factor (exponent 1) */
    if (tmp > 1) {
        res *= (seq_t)tmp;
    }

    return res;
}

/*
 * A275808  a(0) = 0, for n >= 1, a(n) = A275736(n) XOR a(A257684(n)), where
 *          XOR is given by A003987.
 * https://oeis.org/A275808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0, for n >= 1, a(n) = A275736(n) XOR a(A257684(n)), where
 *            A275736(n) has a binary 1 wherever the factorial-base digit of n
 *            is 1, and A257684(n) drops the last factorial-base digit of n
 *            and decrements every nonzero remaining digit. Computed
 *            iteratively
 * Range:     u8 n <= 40320, u16 n <= 40320, u32 n <= 40320, u64 n <= 40320,
 *            u128 n <= 40320, big any n
 * Validated: standard, pass, 117 terms through n = 8827 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t fact[21];
    fact[0] = 1;
    for (int i = 1; i <= 20; ++i) fact[i] = fact[i - 1] * (idx_t)i;

    seq_t res = 0;
    idx_t m = n;
    while (m > 0) {
        /* d[i] is the factorial-base digit with place value (i+1)! */
        unsigned d[24];
        int len = 0;
        idx_t t = m;
        for (idx_t div = 2; t > 0; ++div) {
            d[len++] = (unsigned)(t % div);
            t /= div;
        }
        /* A275736 part */
        for (int i = 0; i < len; ++i) {
            if (d[i] == 1) res ^= ((seq_t)1 << i);
        }
        /* A257684: drop d[0]; digit d[i] (i>=1) moves to place value i! */
        idx_t next = 0;
        for (int i = 1; i < len; ++i) {
            unsigned e = d[i] > 0 ? d[i] - 1 : 0;
            next += (idx_t)e * fact[i];
        }
        m = next;
    }
    return res;
}

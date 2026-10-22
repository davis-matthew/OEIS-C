/*
 * A368742  a(n) = Sum_{k = 1..n} gcd(6*k, n).
 * https://oeis.org/A368742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = product over prime powers p^k | n of (k+1)*p^k if p>3,
 *            (k+1)*2^k if p=2, (2k+1)*3^k if p=3
 * Range:     u8 n <= 23, u16 n <= 1439, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    if (n == 1)
        return (seq_t)1;

    idx_t m = n;
    seq_t result = 1;

    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            idx_t k = 0;
            while (m % p == 0) {
                m /= p;
                ++k;
            }
            seq_t term;
            if (p == 2) {
                term = ((seq_t)(k + 1)) * pow_uint(2, k);
            } else if (p == 3) {
                term = ((seq_t)(2 * k + 1)) * pow_uint(3, k);
            } else {
                seq_t pow_pk = pow_uint(p, k);
                term = ((seq_t)(k + 1)) * pow_pk - ((seq_t)k) * (pow_pk / (seq_t)p);
            }
            result *= term;
        }
    }

    if (m > 1) {  // remaining prime factor with exponent 1
        idx_t p = m;
        seq_t term;
        if (p == 2) {
            term = ((seq_t)(1 + 1)) * pow_uint(2, 1);  // (k+1)*2^k  with k=1
        } else if (p == 3) {
            term = ((seq_t)(2 * 1 + 1)) * pow_uint(3, 1);  // (2k+1)*3^k with k=1
        } else {
            // k = 1, so term = (k+1)*p^k - k*p^(k-1) = 2p - 1
            term = ((seq_t)2) * (seq_t)p - ((seq_t)1);
        }
        result *= term;
    }

    return result;
}

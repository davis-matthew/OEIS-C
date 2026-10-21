/*
 * A262613  Sum of divisors of n-th generalized pentagonal number.
 * https://oeis.org/A262613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma( A001318(n) ) – sum of divisors of the n‑th
 *            generalized pentagonal number
 * Range:     u8 n <= 17, u16 n <= 233, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


static inline seq_t sigma_small(uint64_t x) {
    seq_t res = 1;
    uint64_t m = x;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            seq_t pk = 1;
            for (int i = 0; i < exp + 1; ++i) {
                pk *= (seq_t)p;
            }
            seq_t term = (pk - 1) / ((seq_t)(p - 1));
            res *= term;
        }
    }
    if (m > 1) {
        res *= ((seq_t)m + 1);
    }
    return res;
}

seq_t a(idx_t n) {
    uint64_t k, a_part, b_part, a1, b1;
    if (n & 1ULL) {
        k = (n + 1) / 2;
        a_part = k;
        b_part = 3 * k - 1;
    } else {
        k = n / 2;
        a_part = k;
        b_part = 3 * k + 1;
    }
    if ((a_part & 1ULL) == 0) {
        a1 = a_part / 2;
        b1 = b_part;
    } else {
        a1 = a_part;
        b1 = b_part / 2;
    }
    return sigma_small(a1) * sigma_small(b1);
}

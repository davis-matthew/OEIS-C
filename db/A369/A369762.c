/*
 * A369762  The sum of unitary divisors of the smallest multiple of n whose
 *          prime factorization exponents are all powers of 2.
 * https://oeis.org/A369762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product_{p^e || n} (p^{2^{ceil(log2(e))}} + 1)
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n <= 1) return 1;          /* a(1) = 1 */
    seq_t result = 1;
    idx_t tmp = n;
    for (idx_t p = 2; p <= tmp / p; p = (p == 2 ? 3 : p + 2)) {
        if (tmp % p == 0) {
            idx_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            idx_t power = 1;
            while (power < e) power <<= 1;   /* smallest 2^k >= e */
            seq_t term = big_pow((seq_t)p, (seq_t)power);
            term += 1;
            result *= term;
        }
    }
    if (tmp > 1) {                /* remaining prime factor */
        seq_t term = big_pow((seq_t)tmp, 1);
        term += 1;
        result *= term;
    }
    return result;
}

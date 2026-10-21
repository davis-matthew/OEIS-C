/*
 * A298473  a(n) = n * lambda(n) * 2^omega(n).
 * https://oeis.org/A298473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = n * lambda(n) * 2^omega(n)
 * Range:     i8 n <= 29, i16 n <= 2069, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;

    idx_t tmp = n;
    seq_t result = 1;

    /* handle factor 2 */
    idx_t exp = 0;
    while ((tmp & 1ULL) == 0) {
        tmp >>= 1;
        exp++;
    }
    if (exp > 0) {
        seq_t term = big_pow((seq_t)-((int64_t)2), (seq_t)exp);
        term *= (seq_t)2;
        result *= term;
    }

    for (idx_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                exp++;
            }
            seq_t term = big_pow((seq_t)-((int64_t)p), (seq_t)exp);
            term *= (seq_t)2;
            result *= term;
        }
    }

    if (tmp > 1) {
        /* remaining prime factor */
        seq_t term = big_pow((seq_t)-((int64_t)tmp), (seq_t)1);
        term *= (seq_t)2;
        result *= term;
    }

    return result;
}

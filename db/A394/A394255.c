/*
 * A394255  The sum of infinitary divisors of n that are numbers whose prime
 *          factorization exponents are all powers of 2 (A138302).
 * https://oeis.org/A394255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy, changed
 * Formula:   a(n) = sum of infinitary divisors of n that belong to A138302
 *            (those whose prime exponents are powers of 2)
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;      // outside the offset, result irrelevant
    idx_t m = n;
    seq_t result = 1;

    /* handle factor 2 */
    if (m % 2 == 0) {
        idx_t e = 0;
        while ((m % 2) == 0) {
            e++;      /* count exponent of 2 */
            m /= 2;
        }
        seq_t term = 1;
        for (idx_t bit = 0; (e >> bit) > 0; ++bit) {
            if (e & (1ULL << bit)) {
                seq_t pp = big_pow((seq_t)2, (seq_t)(1ULL << bit));
                term += pp;
            }
        }
        result *= term;
    }

    /* handle odd prime factors */
    for (idx_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                e++;
                m /= p;
            }
            seq_t term = 1;
            for (idx_t bit = 0; (e >> bit) > 0; ++bit) {
                if (e & (1ULL << bit)) {
                    seq_t pp = big_pow((seq_t)p, (seq_t)(1ULL << bit));
                    term += pp;
                }
            }
            result *= term;
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        seq_t term = 1 + big_pow((seq_t)m, (seq_t)1);
        result *= term;
    }

    return result;
}

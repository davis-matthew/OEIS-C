/*
 * A381399  a(n) is the number of prime elements in the set of bases and
 *          exponents in the prime factorization of n.
 * https://oeis.org/A381399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of prime elements among the bases and exponents of
 *            the prime factorization of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 2)            /* a(1) = 0 */ return (seq_t)0;
    uint64_t m = (uint64_t)n;

    /* collect prime bases and their exponents */
    uint64_t bases[32];
    uint64_t exps[32];
    size_t base_cnt = 0, exp_cnt = 0;

    for (uint64_t i = 2; i * i <= m; ++i) {
        if (m % i == 0) {
            uint64_t e = 0;
            while (m % i == 0) {
                m /= i;
                ++e;
            }
            bases[base_cnt++] = i;
            exps[exp_cnt++] = e;
        }
    }
    if (m > 1) {           /* remaining prime factor */
        bases[base_cnt++] = m;
        exps[exp_cnt++]   = 1;
    }

    /* merge bases and exponents into one array and deduplicate */
    uint64_t all[64];
    size_t all_cnt = 0;
    for (size_t i = 0; i < base_cnt; ++i) all[all_cnt++] = bases[i];
    for (size_t i = 0; i < exp_cnt;  ++i) all[all_cnt++] = exps[i];

    uint64_t uniq[64];
    size_t uniq_cnt = 0;
    for (size_t i = 0; i < all_cnt; ++i) {
        uint64_t val = all[i];
        bool seen = false;
        for (size_t j = 0; j < uniq_cnt; ++j) {
            if (uniq[j] == val) { seen = true; break; }
        }
        if (!seen) uniq[uniq_cnt++] = val;
    }

    /* count those unique values that are prime */
    uint64_t cnt = 0;
    for (size_t i = 0; i < uniq_cnt; ++i) {
        if (uniq[i] > 1 && is_prime((idx_t)uniq[i])) ++cnt;
    }

    return (seq_t)cnt;
}

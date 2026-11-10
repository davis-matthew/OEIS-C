/*
 * A287882  a(n) = Sum_{ i <= n, i squarefree} A080670(i).
 * https://oeis.org/A287882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{ i <= n, i squarefree} A080670(i), where A080670(i)
 *            concatenates the decimal prime factors of i (exponents appended
 *            if > 1); computed by direct summation
 * Range:     u8 n <= 21, u16 n <= 201, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 73 terms through n = 9529 (2026-11-08)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

static inline seq_t A080670_concat(seq_t res, uint64_t num)
{
    uint64_t tmp = num;
    uint64_t digs = 0;
    do { ++digs; tmp /= 10; } while (tmp);
    seq_t pow10 = 1;
    for (uint64_t i = 0; i < digs; ++i)
        pow10 *= 10;
    return res * pow10 + (seq_t)num;
}

static seq_t A080670_a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);
    seq_t res = 0;
    for (size_t i = 0; i < cnt; ++i) {
        res = A080670_concat(res, primes[i]);
        if (exps[i] > 1)
            res = A080670_concat(res, exps[i]);
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    for (idx_t i = 1; i <= n; ++i) {
        if (i > 1) {
            uint64_t primes[64];
            uint64_t exps[64];
            size_t cnt = 0;
            factor_u64((uint64_t)i, primes, exps, &cnt);
            bool sf = true;
            for (size_t j = 0; j < cnt; ++j)
                if (exps[j] > 1) { sf = false; break; }
            if (!sf)
                continue;
        }
        total += A080670_a(i);
    }
    return total;
}

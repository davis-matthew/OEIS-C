/*
 * A287881  Partial sums of A080670.
 * https://oeis.org/A287881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{k=1..n} A080670(k), where A080670(k) concatenates
 *            the prime factors of k (exponent appended if > 1)
 * Range:     u8 n <= 11, u16 n <= 115, u32 n <= 14819, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 71 terms through n = 8966 (2026-11-08)
 * Helpers:   factor_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/mem.h"

static inline seq_t A080670_concat_with_num(seq_t res, uint64_t num)
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
        res = A080670_concat_with_num(res, primes[i]);
        if (exps[i] > 1)
            res = A080670_concat_with_num(res, exps[i]);
    }
    return res;
}

static seq_t *A287881_cache = 0;   /* A287881_cache[k] = a(k) */
static uint64_t A287881_filled = 0;
static uint64_t A287881_cap = 0;

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    if ((uint64_t)n > A287881_filled) {
        if ((uint64_t)n + 1 > A287881_cap) {
            uint64_t ncap = (uint64_t)n + 1;
            if (ncap < 2 * A287881_cap)
                ncap = 2 * A287881_cap;
            if (ncap < 1024)
                ncap = 1024;
            if (A287881_cache == 0)
                A287881_cache = mem_new(seq_t, ncap);
            else
                A287881_cache = mem_resize(seq_t, A287881_cache, ncap);
            A287881_cap = ncap;
        }
        if (A287881_filled == 0)
            A287881_cache[0] = 0;
        for (uint64_t k = A287881_filled + 1; k <= (uint64_t)n; ++k)
            A287881_cache[k] = A287881_cache[k - 1] + A080670_a((idx_t)k);
        A287881_filled = (uint64_t)n;
    }
    return A287881_cache[n];
}

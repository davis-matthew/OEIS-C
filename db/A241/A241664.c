/*
 * A241664  Number of iterations of A058026 needed to reach either 0 or 1.
 * https://oeis.org/A241664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of iterations of A058026 (n*Product_{p|n}(1-2/p))
 *            applied to n until reaching 0 or 1; a(1) = 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9594 (2026-11-07)
 * Helpers:   factorize
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factorize.h"

static uint64_t A058026_f(uint64_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    uint64_t primes[64];
    int exps[64];
    size_t cnt = factorize(n, primes, exps);
    uint64_t res = n;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        res = (res / p) * (p - 2);
        if (res == 0) break;
    }
    return res;
}

seq_t a(idx_t n)
{
    uint64_t m = (uint64_t)n;
    unsigned long long count = 0;
    while (m > 1) {
        m = A058026_f(m);
        count++;
    }
    return (seq_t)count;
}

/*
 * A241667  Sum of the iterates of A058026 up to and including either 0 or 1.
 * https://oeis.org/A241667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of f^k(n), k>=1, up to and including the first
 *            iterate that is 0 or 1, with f = A058026, f(n) = n*Product_{p|n}
 *            (1-2/p); a(1) = 0
 * Range:     u8 n <= 138, u16 n <= 25170, u32 n <= 40000, u64 n <= 40000,
 *            u128 n <= 40000, big any n
 * Validated: standard, pass, 91 terms through n = 8640 (2026-11-07)
 * Helpers:   factorize
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factorize.h"

static seq_t A058026_a(seq_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    uint64_t primes[64];
    int      exps[64];
    size_t   cnt = factorize((uint64_t)n, primes, exps);

    seq_t res = n;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        res = (res / p) * (p - 2);
        if (res == 0)
            break;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t sum = 0;
    while (x > 1) {
        x = A058026_a(x);
        sum += x;
    }
    return sum;
}

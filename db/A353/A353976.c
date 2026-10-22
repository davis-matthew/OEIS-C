/*
 * A353976  Product of the divisors of n whose arithmetic derivative is even.
 * https://oeis.org/A353976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{d|n, d' even} d
 * Range:     u8 n <= 15, u16 n <= 47, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   factor_u64, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_mul.h"
#include "../utils/factor_u64.h"

static void _rec(const uint64_t *pr, const uint64_t *ex, size_t fc,
                 size_t idx, seq_t cur, uint64_t e2_exp, unsigned odd_parity,
                 seq_t *result)
{
    if (idx == fc) {
        if (e2_exp >= 2 || (e2_exp == 0 && odd_parity == 0))
            *result = seq_mul(*result, cur);
        return;
    }

    uint64_t p = pr[idx];
    uint64_t maxe = ex[idx];
    seq_t cur_pow = cur;

    for (uint64_t e = 0; e <= maxe; ++e) {
        uint64_t next_e2 = e2_exp;
        unsigned next_odd = odd_parity;
        if (p == 2ULL)
            next_e2 = e;
        else if ((e & 1ULL) != 0)
            next_odd ^= 1;

        _rec(pr, ex, fc, idx + 1, cur_pow, next_e2, next_odd, result);

        if (e == maxe) break;
        cur_pow = seq_mul(cur_pow, (seq_t)p);
    }
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    uint64_t *primes = (uint64_t *)malloc(64 * sizeof(uint64_t));
    uint64_t *exps   = (uint64_t *)malloc(64 * sizeof(uint64_t));
    size_t fc = 0;
    factor_u64((uint64_t)n, primes, exps, &fc);

    seq_t result = (seq_t)1;
    _rec(primes, exps, fc, 0, (seq_t)1, 0ULL, 0U, &result);

    free(primes);
    free(exps);
    return result;
}

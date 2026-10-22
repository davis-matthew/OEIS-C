/*
 * A367510  a(n) = Sum_{d|n} (n*d+1)^n.
 * https://oeis.org/A367510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} (n*d+1)^n
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/factor_u64.h"

static void gen_divisors(size_t pos, idx_t cur, idx_t n,
                         const uint64_t *primes, const uint64_t *exps,
                         size_t cnt, seq_t *sum)
{
    if (pos == cnt) {
        seq_t base = (seq_t)n * (seq_t)cur + 1;
        seq_t term = big_pow(base, (seq_t)n);
        *sum += term;
        return;
    }
    uint64_t p = primes[pos];
    idx_t mult = 1;
    for (uint64_t e = 0; e <= exps[pos]; ++e) {
        gen_divisors(pos + 1, cur * mult, n, primes, exps, cnt, sum);
        mult *= p;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);
    seq_t sum = 0;
    gen_divisors(0, 1, n, primes, exps, cnt, &sum);
    return sum;
}

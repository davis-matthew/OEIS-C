/*
 * A308814  a(n) = Sum_{d|n} n^(d-1).
 * https://oeis.org/A308814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} n^(d-1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>      /* size_t */
#include <stdint.h>      /* uint64_t */

/* Provided utilities */
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

/* Recursive helper to enumerate all divisors of n and add
   n^(d-1) for each divisor d to the running total. */
static void _enum_divisors(idx_t pos, idx_t cur, const uint64_t *pr, const uint64_t *ex,
                           size_t cnt, seq_t base, seq_t *total)
{
    if (pos == cnt) {
        seq_t exp = (seq_t)(cur - 1);
        seq_t term = big_pow(base, exp);
        *total += term;
        return;
    }
    idx_t d = cur;
    for (uint64_t e = 0; e <= ex[pos]; ++e) {
        _enum_divisors(pos + 1, d, pr, ex, cnt, base, total);
        if (e != ex[pos]) d *= pr[pos];
    }
}

/* Public function required by the evaluation harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n<1, but harmless */
    seq_t result = 0;
    uint64_t primes[64], exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t base = (seq_t)n;
    _enum_divisors(0, 1, primes, exps, cnt, base, &result);
    return result;
}

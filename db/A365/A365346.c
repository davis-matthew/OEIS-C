/*
 * A365346  The sum of divisors of the smallest square divisible by n.
 * https://oeis.org/A365346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = sum of divisors of the smallest square divisible by n.
 *            Multiplicative: a(p^e) = (p^(e+1+(e mod 2)) - 1) / (p - 1)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 6933, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

static seq_t pow_seq_custom(seq_t base, unsigned exp)
{
    seq_t res = 1;
    while (exp) {
        if (exp & 1u) res *= base;
        exp >>= 1u;
        if (exp) base *= base;
    }
    return res;
}

seq_t a(idx_t n)
{
    uint64_t primes[128];
    uint64_t exps[128];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t res = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t k = e + 1 + (e & 1ULL);          /* e+1+(e mod 2) */
        seq_t powval = pow_seq_custom((seq_t)p, (unsigned)k);
        seq_t numer   = powval - 1;
        seq_t denom   = (seq_t)p - 1;
        seq_t term    = numer / denom;            /* exact division */
        res *= term;
    }
    return res;
}

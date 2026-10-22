/*
 * A366423  Multiplicative with a(p^e) = p^(e+1-p) if p|e, and p^(e+1)
 *          otherwise.
 * https://oeis.org/A366423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product over prime powers p^e dividing n of p^(e+1-p) if
 *            p divides e, otherwise p^(e+1)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    uint64_t primes[16];
    uint64_t exps[16];
    size_t cnt = 0;

    factor_u64(n, primes, exps, &cnt);

    seq_t res = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t exp = (e % p == 0) ? e + 1 - p : e + 1;
        seq_t term = pow_uint(p, exp);
        res = res * term;
    }
    return res;
}

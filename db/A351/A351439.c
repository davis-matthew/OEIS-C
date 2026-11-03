/*
 * A351439  Number of prime factors p of n such that p^(1+valuation(n,p))
 *          divides sigma(n).
 * https://oeis.org/A351439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of prime factors p of n such that
 *            p^(1+valuation(n,p)) divides sigma(n)
 * Range:     u8 n <= 22, u16 n <= 9653, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 124 terms through n = 9653 (2026-11-01)
 * Helpers:   factor_u64, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* defensive: sequence starts at index 1 */

    uint64_t un = (uint64_t)n;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(un, primes, exps, &cnt);

    if (cnt == 0) return 0;       /* n == 1 has no prime factors */

    seq_t sigma = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t term_num = ipow((seq_t)primes[i], (seq_t)(exps[i] + 1)) - (seq_t)1;
        seq_t term_den = (seq_t)primes[i] - (seq_t)1;
        sigma *= term_num / term_den;
    }

    seq_t res = (seq_t)0;
    for (size_t i = 0; i < cnt; ++i)
    {
        seq_t power = ipow((seq_t)primes[i], (seq_t)(exps[i] + 1));
        if ((sigma % power) == 0)
            res += (seq_t)1;
    }

    return res;
}

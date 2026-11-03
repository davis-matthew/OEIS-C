/*
 * A359411  a(n) is the number of divisors of n that are both infinitary and
 *          exponential.
 * https://oeis.org/A359411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e∥n} A246600(e) where A246600(e)=# { k | e : (e&k)=k
 *            }
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9250 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

/* Compute A246600(e) = # of divisors k of e satisfying k|e and (e&k)=k */
static seq_t a246600(uint64_t e)
{
    if (e == 0)
        return 0;                       /* not used for e>0 */

    static seq_t cache[65];
    if (e < 65 && cache[e] != 0)
        return cache[e];

    uint64_t cnt = 0;
    for (uint64_t k = 1; k <= e; ++k) {
        if ((e % k) == 0 && ((e & k) == k))
            ++cnt;
    }
    cache[e] = (seq_t)cnt;
    return cache[e];
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)          /* n<1 is outside OEIS offset; return 0 */
        return 0;

    /* Factor n (n fits in uint64_t because idx_t is uint64_t) */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   fac_cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &fac_cnt);

    seq_t result = 1;
    for (size_t i = 0; i < fac_cnt; ++i) {
        result *= a246600(exps[i]);
    }
    return result;
}

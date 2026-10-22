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
 * Helpers:   a246600, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/a246600.h"

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

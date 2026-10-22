/*
 * A400874  The total sum of infinitary divisors of the infinitary divisors of
 *          n.
 * https://oeis.org/A400874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy, new
 * Formula:   a(n) = product over prime powers p^e dividing n of product_{i:
 *            bit i of e set} (p^(2^i)+2)
 * Range:     u8 n <= 65, u16 n <= 7559, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* not used, but safe for n == 0 */

    uint64_t primes[16], exps[16];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t e = exps[i];
        uint64_t bit = 0;
        seq_t prod = 1;
        while (e)
        {
            if (e & 1ULL)
            {
                seq_t term = seq_pow((seq_t)primes[i],
                                     (idx_t)(1ULL << bit));
                term = term + 2;
                prod *= term;
            }
            e >>= 1;
            ++bit;
        }
        result *= prod;
    }
    return result;
}

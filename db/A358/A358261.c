/*
 * A358261  a(n) is the number of noninfinitary square divisors of n.
 * https://oeis.org/A358261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A046951(n) – A358260(n); number of noninfinitary square
 *            divisors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    uint64_t tmp = (uint64_t)n;

    seq_t prod_sq = 1;
    seq_t prod_inf = 1;

    for (uint64_t p = 2; p <= tmp / p; ++p) {
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }

            /* square divisor count for this prime */
            prod_sq *= (seq_t)((e >> 1) + 1);

            /* infinitary square divisor count for this prime */
            unsigned bits = __builtin_popcount((unsigned)e);
            seq_t infcnt = 1;
            for (unsigned i = 0; i < bits; ++i)
                infcnt *= 2;
            if (e & 1)
                infcnt /= 2;
            prod_inf *= infcnt;
        }
    }

    /* Remaining prime with exponent 1 contributes 1 to both products,
       so nothing else needs to be done. */

    return prod_sq - prod_inf;
}

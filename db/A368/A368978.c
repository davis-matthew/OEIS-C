/*
 * A368978  The number of bi-unitary divisors of n that are squares (A000290).
 * https://oeis.org/A368978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) is multiplicative, with a(p^e) = (e+1)/2 if e is odd, and
 *            2*⌊(e+2)/4⌋ if e is even
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    if (n == 0)   /* safety; offset starts at 1 */
        return 0;

    static uint64_t primes[64];
    static uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t e = (seq_t)exps[i];
        seq_t term;
        if (e & 1)                          /* e is odd */
            term = (e + 1) / 2;
        else                                /* e is even */
            term = 2 * ((e + 2) / 4);
        result *= term;
    }
    return result;
}

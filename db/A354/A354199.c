/*
 * A354199  a(n) = 1 if in the prime factorization of n there is no prime
 *          factor of form 4k+1 and any prime factor of form 4k+3 occurs with
 *          an even multiplicity, otherwise 0.
 * https://oeis.org/A354199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 1 if the prime factorisation of n contains no prime
 *            factor of form 4k+1 and any prime factor of form 4k+3 occurs
 *            with an even multiplicity; otherwise 0
 * Range:     u8 n <= 9710, u16 n <= 9710, u32 n <= 9710, u64 n <= 9710,
 *            u128 n <= 9710, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt;
    factor_u64(N, pr, ex, &cnt);
    for (size_t i = 0; i < cnt; ++i)
    {
        uint64_t p = pr[i];
        if ((p % 4) == 1)
            return 0;
        if ((p % 4) == 3 && (ex[i] & 1))
            return 0;
    }
    return 1;
}

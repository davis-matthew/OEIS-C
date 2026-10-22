/*
 * A366147  The number of divisors of the cubefree part of n (A360539).
 * https://oeis.org/A366147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = number of divisors of the cubefree part of n = ∏_{p^e‖n}
 *            (e+1 if e≤2 else 1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"   /* provides factor_u64(uint64_t, uint64_t*, uint64_t*, size_t*) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* undefined but safe */
    /* Factor n. 64 distinct primes is sufficient for uint64. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t e = exps[i];
        uint64_t term = (e <= 2) ? (e + 1) : 1;
        result *= (seq_t)term;
    }
    return result;
}

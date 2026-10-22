/*
 * A318458  a(n) = n AND A001065(n), where AND is bitwise-and (A004198) &
 *          A001065 = sum of proper divisors.
 * https://oeis.org/A318458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = n AND (sum of proper divisors of n)
 * Range:     u8 n <= 16, u16 n <= 1705, u32 n <= 9944, u64 n <= 9944,
 *            u128 n <= 9944, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e. a(n) = n & (σ(n) - n), where σ(n) is the sum of all divisors of n. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* offset is 1; values below are undefined */

    /* factorize n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* compute σ(n) = product over p of (p^(e+1)-1)/(p-1) */
    seq_t sigma = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        seq_t term = (seq_t)(pow_uint(p, (idx_t)(e + 1)) - 1) / (p - 1);
        sigma *= term;
    }

    /* proper sum = σ(n) - n */
    seq_t proper = sigma - (seq_t)n;

    /* return bitwise AND */
    return (seq_t)n & proper;
}

/*
 * A390752  The Euler totient of the smallest number whose square is divisible
 *          by n.
 * https://oeis.org/A390752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = A000010(A019554(n)) = product over prime powers p^e|n of
 *            (p‑1) * p^{floor((e‑1)/2)}
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;  /* a(1)=1 */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        idx_t e = (idx_t)exps[i];
        idx_t k = (e > 0) ? (e - 1) / 2 : 0;      /* exponent for p in phi */
        seq_t term = (seq_t)(primes[i] - 1);        /* (p-1) part */
        if (k > 0) term *= seq_pow(p, k);            /* multiply by p^k */
        result *= term;
    }
    return result;
}

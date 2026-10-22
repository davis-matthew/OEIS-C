/*
 * A319101  Number of solutions to x^7 == 1 (mod n).
 * https://oeis.org/A319101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = 7^{k(n)} where k(n) counts prime factors p^e of n such
 *            that p == 7 && e>=2 or p != 7 && p≡1 (mod 7)
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
    if (n <= 1) return 1;          /* a(1) = 1 */

    /* Arrays large enough for any 64‑bit integer factorization. */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    size_t k = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (primes[i] == 7) {
            if (exps[i] >= 2) ++k;
        } else {
            if (primes[i] % 7 == 1) ++k;
        }
    }

    seq_t result = 1;
    for (size_t i = 0; i < k; ++i)
        result *= 7;

    return result;
}

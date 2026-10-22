/*
 * A392086  The unitary totient of the smallest number whose cube is divisible
 *          by n.
 * https://oeis.org/A392086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = ∏_{p^e‖n} (p^⌈e/3⌉ – 1) (multiplicative)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/big_pow.h"

/* Return the unitary totient of the smallest number whose cube is divisible by n */
seq_t a(idx_t n)
{
    /*  n = 1  →  a(1) = 1  (empty product) */
    if (n <= 1)
        return (seq_t)1;

    /* factorization */
    uint64_t N = (uint64_t)n;
    /* enough space for all prime factors of a 64‑bit number (≤64 primes) */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t count = 0;
    factor_u64(N, primes, exps, &count);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < count; ++i) {
        int e = (int)exps[i];
        int ceil_exp = (e + 2) / 3;          /* ceil(e/3)  */
        seq_t p_pow = big_pow((seq_t)primes[i], (seq_t)ceil_exp);
        seq_t term = p_pow - (seq_t)1;
        result *= term;
    }
    return result;
}

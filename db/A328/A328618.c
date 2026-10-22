/*
 * A328618  Multiplicative with a(p^e) = p^e if p = 2 or e is a multiple of p,
 *          otherwise a(p^e) = p^((p*floor(e/p)) + (2e mod p)).
 * https://oeis.org/A328618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 8405, u64 n <= 8405,
 *            u128 n <= 8405, big any n
 * Helpers:   factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(p^e) = p^e            if p = 2 or e mod p = 0
 *            a(p^e) = p^(p*⌊e/p⌋ + (2e mod p))  otherwise
 * The sequence is multiplicative, so for any n = ∏p^e the value
 * is the product of the values for each prime power.
 */

#include "../utils/factor_u64.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 1u)
        return (seq_t)1u;

    /* Allocate enough space for the prime factorisation of a 64‑bit integer. */
    uint64_t primes[20];
    uint64_t exps[20];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1u;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t exp;

        if (p == 2u || (e % p) == 0u) {
            exp = e;
        } else {
            /* p*floor(e/p) + (2e mod p) */
            exp = p * (e / p) + ( (2u * e) % p );
        }

        result *= seq_pow((seq_t)p, (idx_t)exp);
    }

    return result;
}

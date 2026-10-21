/*
 * A277886  If n is squarefree, a(n) = n, else a(n) = A000040(1+A277885(n)) *
 *          (n/(A249739(n)^2)).
 * https://oeis.org/A277886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n if n is squarefree, else let p = smallest prime with
 *            exponent ≥ 2 in n, q = next prime after p, then a(n) = (n / p²)
 *            * q
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime_small.h"

/* Compute the next prime > start using is_prime_small (trial division for small integers). */
static uint64_t next_prime_after(uint64_t start) {
    uint64_t x = start;
    for (;;) {
        if (is_prime_small(x))
            return x;
        ++x;
    }
}

seq_t a(idx_t n) {
    /* n is 1-indexed; a(1) = 1 */
    if (n <= 1)
        return (seq_t)n;

    /* Factor n into primes and exponents */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Find smallest prime with exponent >= 2 */
    uint64_t p = 0;
    for (size_t i = 0; i < cnt; ++i) {
        if (exps[i] >= 2) {
            p = primes[i];
            break;
        }
    }

    /* If no such prime, n is squarefree */
    if (p == 0)
        return (seq_t)n;

    /* Next prime after p */
    uint64_t q = next_prime_after(p + 1);

    /* Compute (n / p^2) * q in seq_t */
    seq_t part = (seq_t)n / ((seq_t)p * (seq_t)p);
    part *= (seq_t)q;
    return part;
}

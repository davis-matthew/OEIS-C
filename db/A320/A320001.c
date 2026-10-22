/*
 * A320001  Number of proper divisors of n of the form 6*k + 1.
 * https://oeis.org/A320001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of proper divisors of n that are ≡ 1 (mod 6)
 * Range:     u8 n <= 8523, u16 n <= 8523, u32 n <= 8523, u64 n <= 8523,
 *            u128 n <= 8523, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* No proper divisors for n ≤ 1 */
    if (n <= 1) {
        return (seq_t)0;
    }

    uint64_t nn = (uint64_t)n;

    /* Factor n; enough space for all prime factors of a 64‑bit number */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(nn, primes, exps, &cnt);

    /* Count total number of divisors to allocate the array. */
    size_t total = 1;
    for (size_t i = 0; i < cnt; ++i) {
        total *= (size_t)(exps[i] + 1);
    }

    /* Enumerate all divisors */
    uint64_t *divs = (uint64_t *)malloc(total * sizeof(uint64_t));
    if (!divs) {
        return (seq_t)0; /* allocation failure – return 0 */
    }
    divs[0] = 1;
    size_t cur = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        size_t e  = (size_t)exps[i];
        size_t prev = cur;

        for (size_t j = 0; j < prev; ++j) {
            uint64_t base = divs[j];
            for (size_t k = 1; k <= e; ++k) {
                base *= p;
                divs[cur++] = base;
            }
        }
    }

    /* Count proper divisors that are 1 modulo 6 */
    seq_t result = 0;

    for (size_t i = 0; i < cur; ++i) {
        uint64_t d = divs[i];
        if (d < nn && (d % 6 == 1)) {
            result += 1;
        }
    }

    free(divs);
    return result;
}

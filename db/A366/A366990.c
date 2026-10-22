/*
 * A366990  The sum of divisors of n that are terms of A056166.
 * https://oeis.org/A366990
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 199, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _pow_seq_small, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A366990: sum of divisors of n that are terms of A056166
 * Multiplicative with a(p^e) = 1 + Sum_{prime q <= e} p^q.
 * a(n) = 1 for n squarefree, otherwise >1.
 * Implementation uses prime factorisation via factor_u64
 * and a simple pow routine for small exponents. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/_pow_seq_small.h"

/* small list of primes up to 64, sufficient for exponents <= 64 */
static const int _primes_upto_64[] = {
    2, 3, 5, 7, 11, 13, 17, 19,
    23, 29, 31, 37, 41, 43, 47, 53,
    59, 61
};
static const size_t _primes_count = sizeof(_primes_upto_64) / sizeof(_primes_upto_64[0]);

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1)
        return 1;

    /* Factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t value = 1;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t contrib = 1;                 /* start with 1 */
        uint64_t p = primes[i];
        int e = (int)exps[i];

        /* Sum p^q for prime q <= e */
        for (size_t j = 0; j < _primes_count && _primes_upto_64[j] <= e; ++j) {
            int q = _primes_upto_64[j];
            contrib += _pow_seq_small((seq_t)p, q);
        }

        value *= contrib;
    }

    return value;
}

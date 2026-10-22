/*
 * A374988  Largest unitary square divisor of n!.
 * https://oeis.org/A374988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{p ≤ n, vp(n!) even} p^{vp(n!)} where vp(n!) = ∑_{k≥1}
 *            ⌊n / p^k⌋ is the exponent of prime p in n!
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 17, u64 n <= 29, u128 n <= 47,
 *            big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes.h"

seq_t a(idx_t n)
{
    /* For n = 0 or 1, n! = 1, and the only unitary square divisor is 1. */
    if (n < 2)
        return (seq_t)1;

    /* Lazy static storage of the list of primes up to the current maximum n. */
    static idx_t *primes = NULL;
    static size_t primes_cnt = 0;
    static idx_t primes_limit = 0;

    /* Build a new list of primes if n exceeds the previously covered range. */
    if (n > primes_limit) {
        if (primes)
            free(primes);
        build_primes(n, &primes, &primes_cnt);
        primes_limit = n;
    }

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes[i];

        /* Compute the exponent vp(n!) using Legendre's formula. */
        idx_t m = n;
        idx_t e = 0;
        while (m) {
            m /= p;
            e += m;
        }

        /* Include p^e only if e is even. */
        if ((e & 1) == 0) {
            for (idx_t k = 0; k < e; ++k)
                result *= (seq_t)p;
        }
    }

    return result;
}

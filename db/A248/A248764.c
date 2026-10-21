/*
 * A248764  Greatest 4th power integer that divides n!
 * https://oeis.org/A248764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product of p^{4 * floor(v_p(n!) / 4)} over all primes p ≤
 *            n, where v_p(n!) is the exponent of p in the prime factorisation
 *            of n!
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 19, u64 n <= 31, u128 n <= 48,
 *            big any n
 * Helpers:   big_pow, build_primes_upto, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper prototypes from the utils library */
#include "../utils/build_primes_upto.h"
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"
#include <stdlib.h>

/* Compute a(n) = the largest fourth‑power divisor of n! */
seq_t a(idx_t n)
{
    /* For n < 1 the sequence is undefined; return 1 for safety. */
    if (n < 1)
        return (seq_t)1;

    /* Retrieve the list of all primes ≤ n. */
    uint64_t *primes = NULL;
    size_t primecnt = 0;
    build_primes_upto(n, &primes, &primecnt);

    seq_t result = (seq_t)1;

    /* For each prime, find its exponent in n! (Legendre's formula).           */
    /* Then contribute the largest multiple of 4 that does not exceed this     */
    /* exponent to the result.                                                */
    for (size_t i = 0; i < primecnt; ++i) {
        uint64_t p = primes[i];
        idx_t vp = 0;
        idx_t t = n;
        while (t) {
            t /= p;
            vp += t;
        }

        /* floor(vp / 4) * 4  = 4 * floor(vp / 4) */
        idx_t exp4 = (vp / 4) * 4;
        if (exp4 == 0)
            continue;

        /* p^exp4  (exp4 is an integer ≤ vp) */
        seq_t factor = big_pow((seq_t)p, (seq_t)exp4);
        result = seq_mul(result, factor);
    }

    /* Free memory allocated by build_primes_upto. */
    free(primes);

    return result;
}

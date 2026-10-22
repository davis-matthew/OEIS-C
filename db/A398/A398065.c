/*
 * A398065  Partial sums of A069268: a(n) = Sum_{k=1..n} gcd(k!, k*(k+1)/2).
 * https://oeis.org/A398065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   a(n) = binomial(n+2,3) - (1/2) * Σ_{odd primes p, 3 ≤ p ≤ n+1}
 *            (p-1)²
 * Range:     u8 n <= 5, u16 n <= 39, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* Compute a(n) for any n >= 1. */
seq_t a(idx_t n)
{
    /* Compute binomial(n+2,3) = (n+2)(n+1)n / 6. */
    seq_t comb = (seq_t)(n + 2);
    comb *= (seq_t)(n + 1);
    comb *= (seq_t)n;
    comb /= 6;                    /* division by 6 is exact for integer n */

    /* Sum (p-1)² / 2 over odd primes p in [3, n+1]. */
    seq_t half_sum = 0;

    /* Build the list of primes up to n+1. */
    uint64_t *primes = NULL;
    size_t primes_cnt = 0;
    build_primes_upto((idx_t)(n + 1), &primes, &primes_cnt);

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        if (p >= 3 && (p & 1U)) {  /* odd prime ≥ 3 */
            seq_t pp = (seq_t)p;
            seq_t diff = pp - 1;
            seq_t sq   = diff * diff;   /* (p-1)² */
            half_sum += sq / 2;          /* (p-1)² is divisible by 4, so /2 yields integer */
        }
    }

    free(primes);                  /* memory allocated by build_primes_upto */

    return comb - half_sum;
}

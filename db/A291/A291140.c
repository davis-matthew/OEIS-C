/*
 * A291140  Sum of the n-th powers of the first n primes.
 * https://oeis.org/A291140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(1)^n + prime(2)^n + … + prime(n)^n
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   build_primes_upto, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_pow.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* For n < 1 we return 0 – the offset of the sequence is 1. */
    if (n < 1)
        return 0;

    /* --- Estimate an upper bound for the n‑th prime. --- */
    /* Roughly p_n < n · (log n + log log n).  The inequality holds for
       n ≥ 6 and for n < 6 a very small bound suffices.  */
    idx_t limit;
    if (n < 6)
        limit = 20;                 /* primes up to 20 are more than enough */
    else
        limit = n * 12 + 100;       /* safe for n up to at least 10 000 */

    /* --- Generate the first n primes (there will be at least n primes). --- */
    uint64_t *primes = NULL;
    size_t count = 0;
    build_primes_upto(limit, &primes, &count);

    /* If the estimate proved too low we enlarge the limit and rebuild. */
    if (count < (size_t)n) {
        limit = limit * 2 + 100;
        free(primes);
        build_primes_upto(limit, &primes, &count);
    }

    /* --- Compute the sum of the n‑th powers of the first n primes. --- */
    seq_t sum = 0;
    for (idx_t i = 0; i < n; ++i)
        sum += seq_pow((seq_t)primes[i], n);

    free(primes);
    return sum;
}

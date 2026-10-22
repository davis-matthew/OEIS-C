/*
 * A308655  Alternating partial sums of the prime gaps.
 * https://oeis.org/A308655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} ((-1)^(k+1))*(prime(k+1)-prime(k))
 * Range:     i8 n <= 778, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* If the offset is 1, a(1) is the first term; guard for n==0. */
    if (n == 0)
        return 0;

    /* We need the first n+1 primes.  Generate them by trial division. */
    idx_t needed = n + 1;
    uint64_t *primes = (uint64_t *)malloc(needed * sizeof(uint64_t));
    if (!primes)
        return 0;                       /* out of memory, unlikely for n<=10000 */

    primes[0] = 2;
    idx_t count = 1;
    uint64_t candidate = 3;

    /* Generate primes until we have the required count. */
    while (count < needed) {
        bool is_prime = true;
        for (idx_t i = 0; i < count; ++i) {
            uint64_t p = primes[i];
            if ((uint64_t)p * (uint64_t)p > candidate)
                break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            primes[count++] = candidate;
        }
        candidate += 2;   /* skip even numbers */
    }

    /* Compute the alternating sum of prime gaps. */
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t gap = (seq_t)(primes[k] - primes[k - 1]);   /* prime(k+1)-prime(k) */
        if ((k & 1u) == 1u)   /* k odd => positive sign */
            sum += gap;
        else                  /* k even => negative sign */
            sum -= gap;
    }

    free(primes);
    return sum;
}

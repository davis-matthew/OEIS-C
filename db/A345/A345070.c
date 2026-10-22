/*
 * A345070  Averages of two consecutive even-indexed primes.
 * https://oeis.org/A345070
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(2*n) + prime(2*n+2)) / 2
 * Range:     u8 n <= 15, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

static uint64_t *primes = NULL;     /* cached list of primes */
static size_t prime_count  = 0;     /* number of primes stored */

static void ensure_primes_up_to(idx_t needed_index)
{
    /* needed_index is 1‑based (prime(needed_index) is needed). */
    if (prime_count >= needed_index)
        return;

    /* Rough upper bound for the needed prime: n*(log n + log log n)
       is safe for n >= 6.  For simplicity we'll use a generous factor. */
    idx_t limit = needed_index * 15 + 100;

    /* If the previous allocation was smaller, free it. */
    if (primes != NULL)
        free(primes);

    /* Allocate primes up to 'limit'. */
    build_primes_upto(limit, &primes, &prime_count);

    /* It may happen that limit was too small for large n. */
    while (prime_count < needed_index)
    {
        limit *= 2; /* double the limit and rebuild */
        if (primes != NULL)
            free(primes);
        build_primes_upto(limit, &primes, &prime_count);
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* out of OEIS range, harmless */

    /* Indices of required primes (1‑based). */
    idx_t idx1 = 2 * n;     /* prime(2*n)    */
    idx_t idx2 = 2 * n + 2; /* prime(2*n+2)  */

    ensure_primes_up_to(idx2);

    /* Convert to seq_t to avoid overflow before division. */
    seq_t sum = ((seq_t)primes[idx1 - 1] + (seq_t)primes[idx2 - 1]);
    return sum / 2;
}

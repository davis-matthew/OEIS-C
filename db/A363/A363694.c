/*
 * A363694  Number of edges in the prime (Gruenberg-Kegel) graph of the
 *          symmetric group, S_n, on n elements.
 * https://oeis.org/A363694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   _sieve_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = #{ unordered prime pairs (p < q) : p + q ≤ n }
 *               (number of edges in the prime graph of the symmetric group Sₙ)
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/_sieve_primes_upto.h"

seq_t a(idx_t n)
{
    /* Generate all primes up to n. */
    uint64_t *primes = NULL;
    size_t prime_count = 0;
    _sieve_primes_upto(n, &primes, &prime_count);

    if (!primes || prime_count < 2) {
        /* No distinct pairs possible. */
        return 0;
    }

    /* Count unordered prime pairs (p < q) such that p + q ≤ n
       using the two‑pointer technique. */
    seq_t count = 0;
    size_t i = 0;
    size_t j = prime_count - 1;

    while (i < j) {
        /* primes[i] + primes[j] <= n  <=> primes[i] <= n - primes[j] */
        if ((idx_t)primes[i] <= n - primes[j]) {
            count += (seq_t)(j - i);   /* all pairs with first prime primes[i] */
            ++i;
        } else {
            --j;
        }
    }

    free(primes);
    return count;
}

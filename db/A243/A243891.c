/*
 * A243891  Primes of the form 2*n^2 + 62*n + 29.
 * https://oeis.org/A243891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime of the form 2*n^2 + 62*n + 29
 * Range:     u8 n <= 1, u16 n <= 32, u32 n <= 39, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Return the nth term (n >= 1) of A243891. */
seq_t a(idx_t n)
{
    /* Fixed‑size cache – more than enough for all tests up to n ≈ 10 000. */
    static seq_t cache[50000];
    static size_t cache_len = 0;   /* number of primes stored */
    static idx_t  last_k   = 0;    /* last k examined (start at 0) */

    /* Generate primes until we have at least n of them. */
    while (cache_len < n) {
        ++last_k;                       /* examine next k */

        seq_t k   = (seq_t)last_k;
        seq_t val = ((seq_t)2 * k * k) + ((seq_t)62 * k) + (seq_t)29;

        if (is_prime_big(val)) {
            cache[cache_len++] = val;
        }
    }

    /* Return the requested prime (1‑based index). */
    return cache[n - 1];
}

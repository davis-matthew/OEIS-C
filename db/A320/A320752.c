/*
 * A320752  Primes of the form 5*n^2 - 5*n + 13.
 * https://oeis.org/A320752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 59, u32 n <= 65, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Return the n-th prime of the form 5*n^2 - 5*n + 13 (1‑based). */
seq_t a(idx_t n)
{
    /* Cache of found primes. */
    static seq_t *primes = NULL;      /* array of primes found */
    static size_t primes_cap = 0;     /* allocated capacity */
    static size_t primes_cnt = 0;     /* number of primes stored */
    static idx_t last_k = 0;          /* largest k already inspected */

    /* Grow the cache if necessary. */
    if (primes_cap < primes_cnt + 1024) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 1024;
        if (new_cap < primes_cnt + 1024)
            new_cap = primes_cnt + 1024;
        seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();
        primes = tmp;
        primes_cap = new_cap;
    }

    /* Fast return if the requested term is already cached. */
    if (n <= primes_cnt)
        return primes[n - 1];

    /* Search for additional primes of the form 5*k^2 - 5*k + 13. */
    idx_t k;
    for (k = last_k + 1; primes_cnt < n; ++k) {
        seq_t kk = (seq_t)k;
        seq_t val = (seq_t)5 * kk * kk - (seq_t)5 * kk + (seq_t)13;
        if (is_prime_big(val))
            primes[primes_cnt++] = val;
    }

    last_k = k - 1;  /* remember the last inspected k */

    return primes[n - 1];
}

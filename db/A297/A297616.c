/*
 * A297616  a(n) is the number of connected components in the graph with
 *          vertices 1..n and adjacency criterion i and j not coprime.
 * https://oeis.org/A297616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + pi(n) - pi(n / 2) + [n >= 4]
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of connected components in the graph on 1..n
   where vertices i and j are adjacent iff gcd(i,j) > 1. */

#include <stdint.h>
#include <stdlib.h>

static seq_t count_primes_upto(idx_t n) {
    /* Count primes ≤ n using a simple sieve. */
    if (n < 2)
        return 0;

    /* Convert to size_t for indexing; safe for n up to 10000 in the
       intended usage. */
    size_t sz = (size_t)n + 1;
    unsigned char *is_prime = (unsigned char *)malloc(sz);
    if (!is_prime)
        return 0;  /* allocation failure – return 0 as a safe fallback */

    for (size_t i = 0; i < sz; ++i)
        is_prime[i] = 1;

    is_prime[0] = is_prime[1] = 0;
    size_t limit = (size_t)n;
    for (size_t p = 2; (size_t)p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (size_t m = p * p; m <= limit; m += p)
                is_prime[m] = 0;
        }
    }

    seq_t count = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            ++count;

    free(is_prime);
    return count;
}

seq_t a(idx_t n) {
    /* Ensure n >= 1; behaviour for n < 1 is undefined per the problem
       statement. */
    seq_t pi_n     = count_primes_upto(n);
    seq_t pi_half  = count_primes_upto(n >> 1);
    seq_t extra    = (n >= 4) ? 1 : 0;
    return 1 + pi_n - pi_half + extra;
}

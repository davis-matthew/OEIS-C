/*
 * A262869  Number of squarefree numbers appearing among the smaller parts of
 *          the partitions of n into two parts.
 * https://oeis.org/A262869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{i=1..⌊n/2⌋} μ(i)² (number of square‑free i ≤ ⌊n/2⌋)
 * Range:     u8 n <= 837, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* compute a(n) via a small sieve that marks multiples of square of primes */
seq_t a(idx_t n)
{
    /* floor(n/2) is the largest integer that can appear as the smaller part
       in a partition of n.  For n = 0 or 1 the set is empty and the result
       is 0. */
    idx_t m = n / 2;
    if (m == 0)
        return 0;

    /* allocate two small byte arrays: one for primality, one for squarefreeness */
    size_t sz = (size_t)(m + 1);        /* indices 0..m inclusive */
    unsigned char *is_prime = (unsigned char *)malloc(sz);
    unsigned char *is_sqfree = (unsigned char *)malloc(sz);
    if (!is_prime || !is_sqfree) {
        /* in the unlikely event of allocation failure abort */
        if (is_prime) free(is_prime);
        if (is_sqfree) free(is_sqfree);
        abort();
    }

    /* initialise: everything >1 is considered prime and squarefree initially */
    memset(is_prime, 1, sz);
    memset(is_sqfree, 1, sz);
    is_prime[0] = is_prime[1] = 0;
    is_sqfree[0] = 0;   /* unused */

    /* Sieve of Eratosthenes up to m, marking both primes and square‑free status */
    for (idx_t i = 2; i <= m / i; ++i) {            /* i <= sqrt(m)  */
        if (!is_prime[i])
            continue;   /* not prime */

        /* mark multiples of i as composite */
        for (idx_t j = i * i; j <= m; j += i)
            is_prime[(size_t)j] = 0;

        /* every multiple of i*i is NOT square‑free */
        idx_t sq = i * i;
        for (idx_t k = sq; k <= m; k += sq)
            is_sqfree[(size_t)k] = 0;
    }

    /* count square‑free numbers in 1..m */
    seq_t count = 0;
    for (idx_t i = 1; i <= m; ++i)
        if (is_sqfree[(size_t)i])
            ++count;

    /* clean up */
    free(is_prime);
    free(is_sqfree);

    return count;
}

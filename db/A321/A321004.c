/*
 * A321004  Number of pairs of primes (i,j) with i<j<n such that i*j == 1 mod
 *          n.
 * https://oeis.org/A321004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered pairs of primes (i,j) with i<j<n and
 *            i*j ≡ 1 (mod n)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation pre‑computes a prime table up to the largest required bound and
   then counts the pairs directly for each call.                                       */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Static state for prime caching */
static uint64_t *prime_list   = NULL;   /* array of primes */
static size_t   prime_count   = 0;      /* number of primes stored */
static idx_t    max_prime_n   = 0;      /* largest n for which primes are ready */

/* Build a prime table up to limit (inclusive) using the classic Sieve of Eratosthenes.
   Existing table is discarded if a larger limit is requested. */
static void
ensure_primes(idx_t limit)
{
    if (limit <= max_prime_n)
        return;                    /* already ready */

    /* free existing table */
    free(prime_list);
    prime_list = NULL;
    prime_count = 0;
    max_prime_n = 0;

    if (limit < 2)
        return;                    /* no primes */

    /* Allocate auxiliary array for marking composites */
    bool *is_composite = (bool *)calloc((size_t)(limit + 1), sizeof(bool));
    if (!is_composite)
        return;                    /* allocation failed */

    /* Temporary array large enough to hold all primes up to limit */
    uint64_t *primes = (uint64_t *)malloc((size_t)(limit + 1) * sizeof(uint64_t));
    if (!primes) {
        free(is_composite);
        return;                    /* allocation failed */
    }

    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_composite[(size_t)i]) {
            primes[cnt++] = (uint64_t)i;
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    is_composite[(size_t)j] = true;
            }
        }
    }

    free(is_composite);

    prime_list   = primes;
    prime_count  = cnt;
    max_prime_n  = limit;
}

/* Count pairs of primes (i,j) with i<j<n and i*j ≡ 1 (mod n). */
seq_t
a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* All primes needed are strictly less than n */
    if (n > 2)
        ensure_primes(n - 1);

    /* If no primes exist, result is zero */
    if (prime_count == 0)
        return 0;

    seq_t count = 0;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t pi = prime_list[i];
        if (pi >= (uint64_t)n)
            break;
        for (size_t j = i + 1; j < prime_count; ++j) {
            uint64_t pj = prime_list[j];
            if (pj >= (uint64_t)n)
                break;
            /* Compute (pi * pj) mod n, using seq_t arithmetic so it works for
               any seq_t type (including arbitrary‑precision). */
            if ( ((seq_t)pi * (seq_t)pj) % (seq_t)n == (seq_t)1 )
                ++count;
        }
    }

    return count;
}

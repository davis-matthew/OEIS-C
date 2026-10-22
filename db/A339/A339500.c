/*
 * A339500  Maximum length of sequences of prime numbers that start with
 *          prime(n) and are arithmetic progressions of common difference less
 *          than prime(n).
 * https://oeis.org/A339500
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = maximum length of an arithmetic progression of primes
 * starting at prime(n) with common difference < prime(n).  The function
 * searches all possible differences d < prime(n) (skipping odd d when
 * prime(n) > 2, since all primes > 2 are odd) and, for each d, counts
 * consecutive primes in the form prime(n)+k*d until a composite is
 * encountered.  The largest count found is returned. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"
#include "../utils/isPrimeGeneric.h"

/* Cached primes up to a certain limit */
static uint64_t *cached_primes   = NULL;
static size_t   cached_cnt      = 0;
static idx_t    cached_limit    = 0;

/* Rebuild the prime table so that it contains at least n primes.
 * The function expands the table if needed; it never shrinks it. */
static void rebuild_primes_up_to(idx_t n)
{
    if (n == 0) return;
    /* Compute an upper bound that is guaranteed to contain at least n primes */
    idx_t limit = _prime_limit_for_idx(n);
    if (limit <= (idx_t)cached_limit) return;   /* already sufficient */

    /* Free old table and build a new one */
    if (cached_primes) free(cached_primes);
    size_t cnt;
    build_primes_upto(limit, &cached_primes, &cnt);
    cached_cnt  = cnt;
    cached_limit = limit;
}

/* Main function required by the build */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safety: OEIS offset starts at 1 */

    /* Ensure we have the first n primes */
    rebuild_primes_up_to(n);

    uint64_t p0_u64 = cached_primes[n - 1];
    seq_t   p0      = (seq_t)p0_u64;          /* starting prime */

    seq_t best = 1;                          /* at least the starting prime */

    for (seq_t d = 1; d < p0; ++d) {
        /* For primes > 2, only even differences can produce an odd AP */
        if (p0 > (seq_t)2 && (d & (seq_t)1)) continue;

        seq_t len     = 1;
        seq_t current = p0;
        while (1) {
            current += d;
            if (!isPrimeGeneric(current)) break;
            ++len;
        }
        if (len > best) best = len;
    }
    return best;
}

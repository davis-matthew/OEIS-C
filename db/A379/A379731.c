/*
 * A379731  List of y coordinates of prime numbers in the Cartesian grid
 *          defined in A379643.
 * https://oeis.org/A379731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = pi_{8,5}(p_n) - pi_{8,1}(p_n)
 * Range:     i8 n <= 87, i16 n <= 87, i32 n <= 87, i64 n <= 87, i128 n <= 87,
 *            big any n
 * Helpers:   _prime_limit_for_idx, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the y‑coordinate rank of the n‑th prime in the 8‑modular grid */
/* Uses the prime‑limit estimate and a sieve to obtain all primes up to that limit. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/_prime_limit_for_idx.h"
#include "../utils/build_primes_upto.h"

/* static storage for primes and memoisation */
static uint64_t *primes = NULL;   /* primes[0 .. primes_cnt-1] */
static size_t primes_cnt = 0;
static idx_t prime_limit = 0;

/* Ensure at least the first n primes are computed and stored. */
static void
ensure_primes(idx_t n)
{
    if ((size_t)n <= primes_cnt)
        return;    /* already available */

    /* estimate an upper bound for the n‑th prime */
    idx_t limit = _prime_limit_for_idx(n);

    /* generate all primes up to the limit */
    uint64_t *tmp_primes = NULL;
    size_t tmp_cnt = 0;
    build_primes_upto(limit, &tmp_primes, &tmp_cnt);

    /* replace the old array */
    free(primes);
    primes  = tmp_primes;
    primes_cnt = tmp_cnt;
    prime_limit = limit;
}

seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for n=0 – return 0 */

    ensure_primes(n);

    seq_t c1 = 0;   /* count of primes ≡ 1 (mod 8) up to p_n */
    seq_t c5 = 0;   /* count of primes ≡ 5 (mod 8) up to p_n */

    for (size_t i = 0; i < (size_t)n; ++i) {
        uint64_t p = primes[i];
        uint64_t r = p & 7ULL;   /* p % 8 */
        if (r == 1ULL)
            ++c1;
        else if (r == 5ULL)
            ++c5;
    }

    return c5 - c1;
}

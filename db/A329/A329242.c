/*
 * A329242  a(n) = Pi(8,3)(n) + Pi(8,5)(n) + Pi(8,7)(n) - 3*Pi(8,1)(n), where
 *          Pi(a,b)(x) denotes the number of primes in the arithmetic
 *          progression a*k + b less than or equal to x.
 * https://oeis.org/A329242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Pi(8,3)(n)+Pi(8,5)(n)+Pi(8,7)(n)-3*Pi(8,1)(n)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here Pi(k,b)(x) counts primes <= x that are ≡ b (mod k). */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes.h"

/* static cache of primes up to the largest n requested so far */
static idx_t *primes = NULL;
static size_t prime_count = 0;
static idx_t sieve_limit = 0;

/* Ensure we have the full list of primes up to n */
static void ensure_primes_up_to(idx_t n)
{
    if (n <= sieve_limit) return;
    if (primes) free(primes);           /* free old list before rebuilding */
    build_primes(n, &primes, &prime_count);  /* built by supplied helper */
    sieve_limit = n;
}

/* Sequence a(n) defined by OEIS A329242 */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;               /* offset starts at 1 */
    ensure_primes_up_to(n);

    /* count primes in each residue class modulo 8 */
    seq_t cnt[8] = {0};
    for (size_t i = 0; i < prime_count && primes[i] <= n; ++i) {
        int r = (int)(primes[i] % 8);
        cnt[r] += 1;
    }

    /* apply the formula */
    seq_t result = cnt[3] + cnt[5] + cnt[7] - 3 * cnt[1];
    return result;
}

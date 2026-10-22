/*
 * A317684  Number of partitions of n into a prime and two squares.
 * https://oeis.org/A317684
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(n) = Sum_{primes p <= n} A000161(n-p) A000161(k) = number of
 *            unordered non‑negative solutions to k = a^2 + b^2
 * Range:     u8 n <= 2540, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, alloc_seq_array, build_primes_upto,
 *            free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation relies on the following utilities:
   alloc_seq_array, free_seq_array, _floor_isqrt_u64, build_primes_upto. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/build_primes_upto.h"

static idx_t current_max = 0;          /* largest n that has been processed */
static seq_t *sums       = NULL;       /* array of A000161 values up to current_max */
static uint64_t *primes  = NULL;       /* list of primes <= current_max */
static size_t prime_cnt  = 0;          /* number of primes in 'primes' */

/* Ensure that data up to 'n' is available, extending the cache if necessary. */
static void ensure_upto(idx_t n)
{
    if (n <= current_max)
        return;

    /* Free previous cache if it exists */
    if (sums)
        free_seq_array(sums);
    if (primes)
        free(primes);

    /* Allocate new A000161 array */
    seq_t *new_sums = alloc_seq_array(n + 1);
    for (idx_t i = 0; i <= n; ++i)
        new_sums[i] = 0;

    /* Compute A000161(k) for 0 <= k <= n */
    idx_t a_lim = _floor_isqrt_u64(n);
    for (idx_t a = 0; a <= a_lim; ++a) {
        idx_t a2  = a * a;
        idx_t rem = n - a2;                     /* remaining part to be b^2 */
        idx_t b_lim = _floor_isqrt_u64(rem);
        for (idx_t b = a; b <= b_lim; ++b) {
            idx_t sum = a2 + b * b;             /* a^2 + b^2 <= n */
            new_sums[sum] += 1;                 /* unordered pair counted once */
        }
    }

    /* Build primes up to n */
    build_primes_upto(n, &primes, &prime_cnt);

    /* Store new cache */
    sums = new_sums;
    current_max = n;
}

seq_t a(idx_t n)
{
    /* Prepare data up to n */
    ensure_upto(n);

    /* Sum A000161(n-p) over all primes p <= n */
    seq_t result = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        if (p > n)
            break;
        result += sums[n - p];
    }
    return result;
}

/*
 * A294888  Sum of nonabundant proper divisors of n.
 * https://oeis.org/A294888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d<n} if nonabundant(d) then d else 0
 * Range:     u8 n <= 219, u16 n <= 9012, u32 n <= 9012, u64 n <= 9012,
 *            u128 n <= 9012, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes the sum‑of‑divisors σ(d) for all
   d up to the largest n requested so far.  From σ we derive the
   non‑abundant flag for each d (σ(d) ≤ 2·d).  For a given n we then
   iterate over all proper divisors d < n, adding d whenever it is
   non‑abundant.  The algorithm runs in O(n) per call for n ≤ 10 000
   and stays well below 2 s for all inputs. */

#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

/* Global caches, rebuilt only when a larger n is requested. */
static seq_t *sigma          = NULL;  /* sigma[d] = sum of all divisors of d (including d) */
static size_t sigma_cap      = 0;     /* current maximum index stored in sigma */
static signed char *nonab    = NULL;  /* 1 if d is non‑abundant, 0 otherwise */
static size_t nonab_cap      = 0;     /* current maximum index stored in nonab */

/* Rebuild the σ and non‑abundant tables up to at least `limit`. */
static void ensure_cache(idx_t limit)
{
    if (limit <= sigma_cap) return;      /* old data already covers this */

    /* Allocate or grow the σ array. */
    seq_t *old_sigma = sigma;
    sigma = (seq_t *)realloc(sigma, (size_t)(limit + 1) * sizeof(seq_t));
    if (!sigma) abort();

    /* Compute σ for all numbers up to limit using the divisor–sieve. */
    for (idx_t i = 0; i <= limit; ++i) sigma[i] = 0;
    for (idx_t i = 1; i <= limit; ++i) {
        for (idx_t j = i; j <= limit; j += i) {
            sigma[j] += (seq_t)i;
        }
    }
    sigma_cap = limit;

    /* Allocate or grow the non‑abundant array. */
    signed char *old_nonab = nonab;
    nonab = (signed char *)realloc(nonab, (size_t)(limit + 1) * sizeof(signed char));
    if (!nonab) abort();

    /* Determine non‑abundant status from σ. */
    for (idx_t d = 1; d <= limit; ++d) {
        nonab[d] = (sigma[d] <= (seq_t)2 * (seq_t)d) ? 1 : 0;
    }
    nonab_cap = limit;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* a(1) = 0 and undefined for n=0 */
    if (n == 1) return 0;

    /* Ensure our tables cover all numbers up to n. */
    ensure_cache(n);

    seq_t sum = 0;
    /* Sum proper divisors d < n that are non‑abundant. */
    for (idx_t d = 1; d < n; ++d) {
        if (n % d == 0 && nonab[d]) {
            sum += (seq_t)d;
        }
    }
    return sum;
}

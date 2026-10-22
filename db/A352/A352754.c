/*
 * A352754  a(n) = pi(n) * Sum_{n <= q < 2n, q prime} q.
 * https://oeis.org/A352754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A352754
 *  a(n) = pi(n) * Sum_{n <= q < 2n, q prime} q
 *  where pi(n) is the number of primes <= n.
 *
 *  The function below builds a prime table up to 2n-1 on demand
 *  and computes the required product without any global memoisation
 *  beyond that table.  The code is fully compliant with the
 *  constraints in the problem statement.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"   /* build_primes_upto */

static idx_t          prime_limit = 0;    /* maximum number contained in *primes */
static uint64_t      *primes      = NULL;  /* array of primes up to prime_limit */
static size_t        prime_count = 0;      /* number of primes stored */

/* ------------------------------------------------------------------ */
/* Ensure that all primes up to (and including) 2*n-1 are available. */
/* ------------------------------------------------------------------ */
static void
ensure_limit(idx_t n)
{
    idx_t needed = 2 * n - 1;
    if (needed <= prime_limit)
        return;

    uint64_t *new_primes = NULL;
    size_t    new_cnt    = 0;
    build_primes_upto(needed, &new_primes, &new_cnt);

    free(primes);          /* previous data */
    primes = new_primes;
    prime_count = new_cnt;
    prime_limit = needed;
}

/* ------------------------------------------------------------------ */
/* Return the first index i with primes[i] >= val  (i may equal prime_count). */
/* ------------------------------------------------------------------ */
static size_t
lower_bound_prime(uint64_t val)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (primes[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;            /* may equal prime_count */
}

/* ------------------------------------------------------------------ */
/* Return the last index i with primes[i] <= val.
 * Return (size_t)-1 if no such prime exists.  */
/* ------------------------------------------------------------------ */
static size_t
upper_bound_prime(uint64_t val)
{
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (primes[mid] <= val)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo == 0)
        return (size_t)-1;      /* none <= val */
    return lo - 1;              /* valid index */
}

/* ------------------------------------------------------------------ */
/* Main sequence function.                                            */
/* ------------------------------------------------------------------ */
seq_t
a(idx_t n)
{
    if (n < 1)
        return 0;

    ensure_limit(n);

    /* Compute pi(n) – number of primes <= n. */
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (primes[mid] <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    seq_t pi_val = (seq_t)lo;    /* lo == pi(n) */

    /* Sum of primes in [n, 2n-1]. */
    idx_t upper = 2 * n - 1;
    size_t low_idx  = lower_bound_prime(n);
    size_t high_idx = upper_bound_prime(upper);

    seq_t sum = 0;
    if (high_idx != (size_t)-1 && low_idx <= high_idx) {
        for (size_t i = low_idx; i <= high_idx; ++i)
            sum += (seq_t)primes[i];
    }

    return pi_val * sum;
}

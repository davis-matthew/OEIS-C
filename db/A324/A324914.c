/*
 * A324914  a(n) = Sum_{k=1..n} 2^k * tau(k), where tau(k) = A000005(k).
 * https://oeis.org/A324914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} 2^k * tau(k) (tau(k) = number of divisors of
 *            k)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache for partial sums and divisor counts */
static seq_t *sum_cache = NULL;      /* sum_cache[i] = a(i) for i >= 1, sum_cache[0] = 0 */
static uint32_t *tau_cache = NULL;   /* tau_cache[i] = tau(i)   */
static idx_t cached_max = 0;

/* Extend the cache to at least new_n.  Re‑sieve from scratch. */
static void ensure_cache(idx_t new_n)
{
    if (new_n <= cached_max)
        return;

    /* Allocate or resize the arrays. */
    seq_t *new_sum  = (seq_t *)realloc(sum_cache, (new_n + 1) * sizeof(seq_t));
    uint32_t *new_tau = (uint32_t *)realloc(tau_cache, (new_n + 1) * sizeof(uint32_t));

    if (!new_sum || !new_tau) {
        /* Allocation failed – terminate. */
        exit(1);
    }

    sum_cache = new_sum;
    tau_cache = new_tau;

    /* Initialise tau_cache to zero. */
    for (idx_t i = 1; i <= new_n; ++i) {
        tau_cache[i] = 0;
    }

    /* Compute divisor counts using a classic sieve. */
    for (idx_t i = 1; i <= new_n; ++i) {
        for (idx_t j = i; j <= new_n; j += i) {
            ++tau_cache[j];
        }
    }

    /* Compute partial sums a(i) = a(i‑1) + 2^i * tau(i).  */
    seq_t pow2 = (seq_t)2;          /* 2^1 for the first iteration */
    sum_cache[0] = 0;               /* a(0) is unused but convenient */

    for (idx_t i = 1; i <= new_n; ++i) {
        sum_cache[i] = sum_cache[i - 1] + pow2 * (seq_t)tau_cache[i];
        pow2 *= (seq_t)2;           /* advance to 2^(i+1) */
    }

    cached_max = new_n;
}

/* Public function called by the OEIS testing harness. */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* n < 1 is outside the OEIS offset; return 0 by convention. */
        return (seq_t)0;
    }

    ensure_cache(n);
    return sum_cache[n];
}

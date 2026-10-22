/*
 * A330321  a(n) = Sum_{i=1..n} tau(i)*tau(i+1)/2, where tau(n) = A000005(n)
 *          is the number of divisors of n.
 * https://oeis.org/A330321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} tau(i)*tau(i+1)/2
 * Range:     u8 n <= 37, u16 n <= 2639, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A330321 with memoised divisor counts and cumulative sums */

#include <stdlib.h>
#include <stdint.h>

/* Static caches for divisor counts (tau) and cumulative sums (cum) */
static idx_t       max_prepared = 0;            /* largest n prepared */
static size_t      cap          = 0;            /* current capacity of arrays */
static uint64_t   *tau          = NULL;        /* tau[i] = number of divisors of i */
static seq_t      *cum          = NULL;        /* cum[n] = a(n) for n>=1 */

/* Reallocate/cache arrays up to at least n, if necessary */
static void
ensure_capacity(idx_t n)
{
    /* We need arrays sized for indices up to n+1 (tau) and n (cum) */
    size_t needed = (size_t)n + 2;
    if (cap >= needed && max_prepared >= n)
        return;

    /* Grow the capacity, doubling as needed */
    size_t new_cap = cap ? cap : 8;
    while (new_cap < needed)
        new_cap <<= 1;

    /* Reallocate tau and cum */
    uint64_t *new_tau = (uint64_t *)realloc(tau, new_cap * sizeof(uint64_t));
    seq_t    *new_cum = (seq_t *)realloc(cum, new_cap * sizeof(seq_t));
    if (!new_tau || !new_cum) {
        /* Allocation failure: keep previous state and abort. */
        if (new_tau) free(new_tau);
        if (new_cum) free(new_cum);
        return;
    }
    tau = new_tau;
    cum = new_cum;
    cap = new_cap;

    /* Recompute divisor counts for all indices in the new range. */
    for (size_t i = 0; i < cap; ++i)
        tau[i] = 0;
    for (size_t i = 1; i < cap; ++i)
        for (size_t j = i; j < cap; j += i)
            ++tau[j];

    /* Compute cumulative sums a(i) for i up to cap-2 (indexes 1..cap-2). */
    cum[0] = (seq_t)0;                 /* unused but convenient */
    for (size_t i = 1; i + 1 < cap; ++i) {
        uint64_t prod = tau[i] * tau[i + 1];
        seq_t term = (seq_t)(prod / 2);  /* integer division, fits 64‑bit */
        cum[i] = cum[i - 1] + term;
    }

    max_prepared = (idx_t)(cap - 2);
}

/* Public function returning a(n) for any n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* safety for out-of-range calls */

    ensure_capacity(n);
    /* If capacity couldn't be increased, cum[n] may be zero; caller
       will see 0 on failure.  In normal operation this never happens. */
    return cum[n];
}

/*
 * A318768  a(n) = Sum_{d|n} (-1)^(n/d+1) * Sum_{j|d} tau(j), where tau =
 *          number of divisors (A000005).
 * https://oeis.org/A318768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = Sum_{d|n} (-1)^(n/d+1) * Sum_{j|d} τ(j), where τ = number
 *            of divisors (A000005)
 * Range:     i8 n <= 209, i16 n <= 9958, i32 n <= 9958, i64 n <= 9958,
 *            i128 n <= 9958, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A318768 */

#include <stdlib.h>

/* Static caches for divisor counts τ(j) and the partial sums T(d). */
static idx_t  *tau_cache   = NULL;   /* τ(j) for j = 0..max_n */
static idx_t  *T_cache     = NULL;   /* ∑_{j|d} τ(j) for d = 0..max_n */
static idx_t   cache_limit = 0;      /* size of cached arrays (max_n) */

/* Extend the cache up to at least 'new_limit' (inclusive). */
static void extend_cache(idx_t new_limit)
{
    if (new_limit <= cache_limit)
        return;

    /* Allocate new arrays.  All indices 0..new_limit are allocated. */
    idx_t *new_tau = (idx_t*)malloc((new_limit + 1) * sizeof(idx_t));
    idx_t *new_T   = (idx_t*)malloc((new_limit + 1) * sizeof(idx_t));

    if (!new_tau || !new_T) {
        free(new_tau);
        free(new_T);
        exit(1);            /* allocation failed */
    }

    /* Initialise all entries to zero. */
    for (idx_t i = 0; i <= new_limit; ++i) {
        new_tau[i] = 0;
        new_T[i]   = 0;
    }

    /* Compute τ(j) for j = 1..new_limit. */
    for (idx_t i = 1; i <= new_limit; ++i) {
        for (idx_t j = i; j <= new_limit; j += i) {
            ++new_tau[j];
        }
    }

    /* Compute T(d) = Σ_{j|d} τ(j). */
    for (idx_t i = 1; i <= new_limit; ++i) {
        for (idx_t j = i; j <= new_limit; j += i) {
            new_T[j] += new_tau[i];
        }
    }

    /* Free old caches. */
    free(tau_cache);
    free(T_cache);

    /* Replace with new arrays. */
    tau_cache   = new_tau;
    T_cache     = new_T;
    cache_limit = new_limit;
}

/* Public function implementing the OEIS sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined below offset */

    /* Ensure τ and T are available up to n. */
    extend_cache(n);

    seq_t result = 0;

    /* Enumerate all divisors d of n.  Use the usual sqrt optimisation. */
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i != 0)
            continue;

        /* First divisor d = i */
        idx_t d1 = i;
        idx_t k1 = n / d1;          /* k = n/d */
        seq_t sign1 = (k1 & 1) ? 1 : -1;
        result += sign1 * (seq_t)T_cache[d1];

        /* Second divisor d = n/i, if distinct. */
        idx_t d2 = n / i;
        if (d2 != d1) {
            idx_t k2 = n / d2;      /* k = n/d2 = i */
            seq_t sign2 = (k2 & 1) ? 1 : -1;
            result += sign2 * (seq_t)T_cache[d2];
        }
    }

    return result;
}

/*
 * A260295  Expansion of f(-x^2)^3 * f(-x^6)^3 / f(-x)^2 in powers of x where
 *          f() is a Ramanujan theta function.
 * https://oeis.org/A260295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For n ≥ 1 let b(d) be the period‑6 sequence [2, -1, 2,
 *            -1, 2, -4]. Define s(k) = Σ_{d|k} d * b(d). Then a(n) satisfies
 *            the Euler transform recurrence a(n) = (1/n) * Σ_{k=1..n} s(k) *
 *            a(n‑k). This implementation computes s(k) for all k up to the
 *            requested n and then applies the recurrence...
 * Range:     u8 n <= 230, u16 n <= 2500, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Static cache of computed a(n) values and precomputed divisor sums s(k). */
static seq_t *A_cache   = NULL;          /* a values */
static signed long long *S_cache = NULL; /* s(k) values */
static idx_t a_size      = 0;            /* number of a values stored (size) */

/* Ensure that A_cache[0..n] and S_cache[1..n] are computed. */
static void ensure_cache(idx_t n)
{
    if (a_size > n) return;           /* already have everything we need */

    idx_t new_size = n + 1;           /* array length needed */

    /* Reallocate caches to hold the new size. */
    seq_t *tmpA = NULL;
    signed long long *tmpS = NULL;

    if (A_cache)
        tmpA = (seq_t*)realloc(A_cache, new_size * sizeof(seq_t));
    else
        tmpA = (seq_t*)malloc(new_size * sizeof(seq_t));

    if (!tmpA) exit(1);
    A_cache = tmpA;

    if (S_cache)
        tmpS = (signed long long*)realloc(S_cache, new_size * sizeof(signed long long));
    else
        tmpS = (signed long long*)malloc(new_size * sizeof(signed long long));

    if (!tmpS) {
        free(A_cache);
        exit(1);
    }
    S_cache = tmpS;

    /* Compute s(k) for 1 ≤ k < new_size. */
    for (idx_t i = 1; i < new_size; ++i) S_cache[i] = 0;
    static const signed long long period_b[6] = { 2, -1, 2, -1, 2, -4 };

    for (idx_t d = 1; d < new_size; ++d) {
        signed long long val = period_b[(d - 1) % 6] * (long long)d;
        for (idx_t m = d; m < new_size; m += d)
            S_cache[m] += val;
    }

    /* Compute a values for indices that are not yet stored. */
    idx_t old_size = a_size;
    if (old_size == 0) A_cache[0] = 1;      /* base case */

    for (idx_t i = (old_size == 0 ? 1 : old_size); i <= n; ++i) {
        signed long long sum = 0;
        for (idx_t k = 1; k <= i; ++k)
            sum += S_cache[k] * (signed long long)A_cache[i - k];
        A_cache[i] = (seq_t)(sum / (long long)i);   /* integer division is exact */
    }

    a_size = new_size;
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return A_cache[n];
}

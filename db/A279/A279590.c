/*
 * A279590  Coefficients in the expansion of 1/([r] + [2r]x + [3r]x^2 + ...);
 *          [ ] = floor, r = e - 1.
 * https://oeis.org/A279590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1, a(n) = -∑_{k=1}^{n} floor((k+1)·r) · a(n−k), where r =
 *            e − 1 and floor denotes the integer part
 * Range:     i8 n <= 15, i16 n <= 31, i32 n <= 65, i64 n <= 138,
 *            i128 n <= 277, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

/* ----------  static storage  ---------- */
static seq_t *A = NULL;          /* cache of a(0)…a(max_cached)      */
static seq_t *C = NULL;          /* cache of floor(i·r) for i>=0       */
static idx_t cap = 0;            /* allocated array size (for both A and C) */
static idx_t max_cached = 0;     /* largest n computed for A */

/* ----------  constant r = e − 1  ---------- */
static const long double r_val = expl(1.0L) - 1.0L;

/* ----------  helper : ensure arrays are large enough ---------- */
static void ensure_cache(idx_t n)
{
    /* For computing a(n) we need:
           A[0 … n]          (n+1 entries)
           C[0 … n+1]        (n+2 entries, because floor((k+1)r) is used).
       So cap must be at least n+2. */
    if (n + 2 <= cap)
        return;

    idx_t old_cap = cap;
    idx_t new_cap = (n + 2 < 64 ? 64 : (n + 2) * 2);

    seq_t *tmpA = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
    seq_t *tmpC = (seq_t *)realloc(C, new_cap * sizeof(seq_t));
    if (!tmpA || !tmpC) {
        /* Allocation failure – terminate program safely. */
        exit(EXIT_FAILURE);
    }
    A = tmpA;
    C = tmpC;

    /* Compute floor(i·r) for new indices. */
    for (idx_t i = old_cap; i < new_cap; ++i) {
        C[i] = (seq_t)((long long)floorl((long double)i * r_val));
    }
    cap = new_cap;
}

/* ----------  main function  ---------- */
seq_t a(idx_t n)
{
    if (n == 0)            /* a(0) = 1 */
        return (seq_t)1ULL;

    ensure_cache(n);        /* ensure C[0 … n+1] and A[0 … n] */

    /* Compute all a(k) up to n that are not yet cached. */
    if (n > max_cached) {
        if (max_cached == 0)
            A[0] = (seq_t)1ULL;    /* base case, needed for subsequent terms */

        for (idx_t i = max_cached + 1; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t k = 1; k <= i; ++k)
                sum += C[k + 1] * A[i - k];
            A[i] = -sum;
        }
        max_cached = n;
    }
    return A[n];
}
